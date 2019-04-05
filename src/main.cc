#include <iostream>
#include <fstream>
#include <string>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#define _BSD_SOURCE
#include <netdb.h>

#include "OpenApiMessages.pb.h"
#include "OpenApiModelMessages.pb.h"
#include "OpenApiCommonMessages.pb.h"
#include "OpenApiCommonModelMessages.pb.h"
#include "OpenApiMessagesFactory.h"
#include "credentials.h"

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

using namespace google::protobuf::io;
using namespace std;

// ctrader vars
string clientId;
string clientSecret;
string accessToken;
long _accountID;
string _apiHost = "demo.ctraderapi.com";
int _apiPort = 5035;
int MaxMessageSize = 1000000;
long testOrderId = -1;
long testPositionId = -1;
//IList<ProtoOACtidTraderAccount> _accounts;
//List<ProtoOATrader> _traders;

pthread_t thread_sslread;
pthread_attr_t tattr;

// SSL vars
int sd;
SSL_CTX* ctx;
SSL*     ssl;
char buf[4096];


void printfCertInfo(SSL *sslx)
{
    char*    str;
    X509*    server_cert;
    //EVP_PKEY *pubkey;
    //int pkeyLen;
    //unsigned char *ucBuf, *uctempBuf;

    server_cert = SSL_get_peer_certificate (sslx);
    CHK_NULL(server_cert);
    printf ("Server certificate:\n");

    str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t subject: %s\n", str);
    OPENSSL_free (str);

    str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t issuer: %s\n", str);

    /*pubkey = X509_get_pubkey (server_cert);
    pkeyLen = i2d_PublicKey(pubkey, NULL);
    ucBuf = (unsigned char *)malloc(pkeyLen+1);
    uctempBuf = ucBuf;
    i2d_PublicKey(pubkey, &uctempBuf);
    for (int i = 0; i < pkeyLen; i++)
    {
        printf("%02x ", (unsigned char) ucBuf[i]);
    }
    free(ucBuf);
    putchar('\n');*/

    OPENSSL_free (str);

    /* We could do all sorts of certificate verification stuff here before
        deallocating the certificate. */

    X509_free (server_cert);
}

int writeSSLSocket(SSL *sslx, char *msg)
{
    int err = 0;
    /* --------------------------------------------------- */
    /* DATA EXCHANGE - Send a message and receive a reply. */
    err = SSL_write (sslx, msg, strlen(msg));
    CHK_SSL(err);

    return err;
}

int readSSLSocket(SSL *sslx)
{
    memset(buf, 0, sizeof(buf));
    int err = SSL_read (sslx, buf, sizeof(buf) - 1);
    CHK_SSL(err);
    //buf[err] = '\0';
    return err;
}

int openSSLSocket()
{
    int err = 0;
    struct hostent *host_entry;
    struct sockaddr_in sa;
    const SSL_METHOD *meth = TLS_client_method();

    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new (meth);
    CHK_NULL(ctx);

    /* ----------------------------------------------- */
    /* Create a socket and connect to server using normal socket calls. */

    sd = socket (AF_INET, SOCK_STREAM, 0);
    CHK_ERR(sd, "socket");

    memset(&sa, 0, sizeof(sa));
    host_entry = gethostbyname(_apiHost.c_str());
    sa.sin_family      = AF_INET;
    sa.sin_addr        = *((struct in_addr *)host_entry->h_addr);
    sa.sin_port        = htons(_apiPort);

    err = connect(sd, (struct sockaddr*) &sa,
        sizeof(sa));
    CHK_ERR(err, "connect");

    /* ----------------------------------------------- */
    /* Now we have TCP conncetion. Start SSL negotiation. */
    ssl = SSL_new (ctx);
    CHK_NULL(ssl);    
    SSL_set_fd (ssl, sd);
    err = SSL_connect (ssl);
    CHK_SSL(err);

    /* Get the cipher - opt */
    printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

    /* Get server's certificate (note: beware of dynamic allocation) - opt */
    printfCertInfo(ssl);

    return err;
}



void transmit(ProtoMessage msg)
{
    string msgStr;
    int siz = msg.ByteSize();
    char *pkt = new char [siz];
    google::protobuf::io::ArrayOutputStream aos(pkt,siz);
    //
    printf("Msg(%d): ", siz);
    for(int i = 0; i < 5; i++) {
        printf("%d ", pkt[i]);
    }
    cout << "\n";
    writeSSLSocket(ssl, pkt);
}

void authorizeApplication()
{
    OpenApiMessagesFactory msgFactory;
    ProtoMessage msg = msgFactory.CreateAppAuthorizationRequest(clientId,
                        clientSecret);
    transmit(msg);
}

void *read_task(void *arg)
{
    int ret = 0;
    int lenght, num;
    OpenApiMessagesFactory msgFactory;
    ProtoMessage protoMessage;
    while (1)
    {
        lenght = 0;
        ret = readSSLSocket(ssl);
        if (ret > 0) {
            memcpy(&num, buf, 4);
            // big endian to little endian
            lenght = ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); // byte 0 to byte 3
        }
        if (lenght > 0) {
            printf("lenght: %d\n", lenght);
            if (lenght > MaxMessageSize) {
                cout << "Invalid lenght\n";
                continue;
            }
            ret = readSSLSocket(ssl);
            if (ret != lenght)
                continue;
            string _message(buf);
            protoMessage = msgFactory.GetMessage(_message);
            //
            switch ((ProtoOAPayloadType)protoMessage.payloadtype())
            {
                case PROTO_OA_EXECUTION_EVENT:
                    cout << "Event\n";
                    /*var _payload_msg = msgFactory.GetExecutionEvent(_message);
                    if (_payload_msg.HasOrder)
                    {
                        testOrderId = _payload_msg.Order.OrderId;
                    }
                    if (_payload_msg.HasPosition)
                    {
                        testPositionId = _payload_msg.Position.PositionId;
                    }*/
                    break;

                case PROTO_OA_GET_ACCOUNTS_BY_ACCESS_TOKEN_RES:
                    cout << "TokenRes\n";
                    //var _accounts_list = ProtoOAGetAccountListByAccessTokenRes.CreateBuilder().MergeFrom(protoMessage.Payload).Build();
                    //_accounts = _accounts_list.CtidTraderAccountList;

                    break;

                case PROTO_OA_TRADER_RES:
                    cout << "TraderResp\n";
                    //var trader = ProtoOATraderRes.CreateBuilder().MergeFrom(protoMessage.Payload).Build();
                    //_traders.Add(trader.Trader);
                    break;

                default:
                    break;
            }
            //
        }
        usleep(100000);
    }
    pthread_exit(&ret);
}

int main(int argc, char* argv[])
{
    char opt;
    clientId = CLIID;
    clientSecret = CLSECRET;
    accessToken = ACCTOKEN;
    _accountID = ACCID;

    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
    //
    openSSLSocket();
    pthread_create(&thread_sslread, &tattr, read_task, NULL);
    while (1) {
        cout << "1 - Authorize App\n2 - Authorize Account\n";
        cin >> opt;
        switch(opt)
        {
            case '1':
                authorizeApplication();
                break;
        }
    }

    SSL_shutdown (ssl);  /* send SSL/TLS close_notify */
    /* Clean up. */
    close (sd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);

    return 0;
}
