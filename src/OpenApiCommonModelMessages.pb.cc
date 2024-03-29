// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: OpenApiCommonModelMessages.proto

#include "OpenApiCommonModelMessages.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_OpenApiCommonModelMessages_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_OpenApiCommonModelMessages_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_OpenApiCommonModelMessages_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_OpenApiCommonModelMessages_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_OpenApiCommonModelMessages_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n OpenApiCommonModelMessages.proto*I\n\020Pr"
  "otoPayloadType\022\021\n\rPROTO_MESSAGE\020\005\022\r\n\tERR"
  "OR_RES\0202\022\023\n\017HEARTBEAT_EVENT\0203*\360\001\n\016ProtoE"
  "rrorCode\022\021\n\rUNKNOWN_ERROR\020\001\022\027\n\023UNSUPPORT"
  "ED_MESSAGE\020\002\022\023\n\017INVALID_REQUEST\020\003\022\021\n\rTIM"
  "EOUT_ERROR\020\005\022\024\n\020ENTITY_NOT_FOUND\020\006\022\026\n\022CA"
  "NT_ROUTE_REQUEST\020\007\022\022\n\016FRAME_TOO_LONG\020\010\022\021"
  "\n\rMARKET_CLOSED\020\t\022\033\n\027CONCURRENT_MODIFICA"
  "TION\020\n\022\030\n\024BLOCKED_PAYLOAD_TYPE\020\013BM\n(com."
  "xtrader.protocol.proto.commons.modelB\034Co"
  "ntainerCommonModelMessagesP\001\240\001\001"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_OpenApiCommonModelMessages_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_OpenApiCommonModelMessages_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_OpenApiCommonModelMessages_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_OpenApiCommonModelMessages_2eproto = {
  false, false, descriptor_table_protodef_OpenApiCommonModelMessages_2eproto, "OpenApiCommonModelMessages.proto", 431,
  &descriptor_table_OpenApiCommonModelMessages_2eproto_once, descriptor_table_OpenApiCommonModelMessages_2eproto_sccs, descriptor_table_OpenApiCommonModelMessages_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_OpenApiCommonModelMessages_2eproto::offsets,
  file_level_metadata_OpenApiCommonModelMessages_2eproto, 0, file_level_enum_descriptors_OpenApiCommonModelMessages_2eproto, file_level_service_descriptors_OpenApiCommonModelMessages_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_OpenApiCommonModelMessages_2eproto(&descriptor_table_OpenApiCommonModelMessages_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ProtoPayloadType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_OpenApiCommonModelMessages_2eproto);
  return file_level_enum_descriptors_OpenApiCommonModelMessages_2eproto[0];
}
bool ProtoPayloadType_IsValid(int value) {
  switch (value) {
    case 5:
    case 50:
    case 51:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ProtoErrorCode_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_OpenApiCommonModelMessages_2eproto);
  return file_level_enum_descriptors_OpenApiCommonModelMessages_2eproto[1];
}
bool ProtoErrorCode_IsValid(int value) {
  switch (value) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
