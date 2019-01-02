/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "messenger_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace thrift_gen_messenger {


Message::~Message() throw() {
}


void Message::__set__timestamp(const std::string& val) {
  this->_timestamp = val;
}

void Message::__set__sender_id(const std::string& val) {
  this->_sender_id = val;
}

void Message::__set__receiver_id(const std::string& val) {
  this->_receiver_id = val;
__isset._receiver_id = true;
}

void Message::__set__subject(const std::string& val) {
  this->_subject = val;
__isset._subject = true;
}

void Message::__set__sequence_no(const int64_t val) {
  this->_sequence_no = val;
__isset._sequence_no = true;
}

void Message::__set__total_count(const int64_t val) {
  this->_total_count = val;
__isset._total_count = true;
}

void Message::__set__list_bool(const std::vector<bool> & val) {
  this->_list_bool = val;
__isset._list_bool = true;
}

void Message::__set__list_i16(const std::vector<int16_t> & val) {
  this->_list_i16 = val;
__isset._list_i16 = true;
}

void Message::__set__list_i32(const std::vector<int32_t> & val) {
  this->_list_i32 = val;
__isset._list_i32 = true;
}

void Message::__set__list_i64(const std::vector<int64_t> & val) {
  this->_list_i64 = val;
__isset._list_i64 = true;
}

void Message::__set__list_double(const std::vector<double> & val) {
  this->_list_double = val;
__isset._list_double = true;
}

void Message::__set__list_string(const std::vector<std::string> & val) {
  this->_list_string = val;
__isset._list_string = true;
}

void Message::__set__binary(const std::string& val) {
  this->_binary = val;
__isset._binary = true;
}

void Message::__set__payload(const std::string& val) {
  this->_payload = val;
__isset._payload = true;
}
std::ostream& operator<<(std::ostream& out, const Message& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Message::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset__timestamp = false;
  bool isset__sender_id = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_timestamp);
          isset__timestamp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_sender_id);
          isset__sender_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_receiver_id);
          this->__isset._receiver_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_subject);
          this->__isset._subject = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->_sequence_no);
          this->__isset._sequence_no = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->_total_count);
          this->__isset._total_count = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_bool.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->_list_bool.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += iprot->readBool(this->_list_bool[_i4]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_bool = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_i16.clear();
            uint32_t _size5;
            ::apache::thrift::protocol::TType _etype8;
            xfer += iprot->readListBegin(_etype8, _size5);
            this->_list_i16.resize(_size5);
            uint32_t _i9;
            for (_i9 = 0; _i9 < _size5; ++_i9)
            {
              xfer += iprot->readI16(this->_list_i16[_i9]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_i16 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_i32.clear();
            uint32_t _size10;
            ::apache::thrift::protocol::TType _etype13;
            xfer += iprot->readListBegin(_etype13, _size10);
            this->_list_i32.resize(_size10);
            uint32_t _i14;
            for (_i14 = 0; _i14 < _size10; ++_i14)
            {
              xfer += iprot->readI32(this->_list_i32[_i14]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_i32 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_i64.clear();
            uint32_t _size15;
            ::apache::thrift::protocol::TType _etype18;
            xfer += iprot->readListBegin(_etype18, _size15);
            this->_list_i64.resize(_size15);
            uint32_t _i19;
            for (_i19 = 0; _i19 < _size15; ++_i19)
            {
              xfer += iprot->readI64(this->_list_i64[_i19]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_i64 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 11:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_double.clear();
            uint32_t _size20;
            ::apache::thrift::protocol::TType _etype23;
            xfer += iprot->readListBegin(_etype23, _size20);
            this->_list_double.resize(_size20);
            uint32_t _i24;
            for (_i24 = 0; _i24 < _size20; ++_i24)
            {
              xfer += iprot->readDouble(this->_list_double[_i24]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_double = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 12:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->_list_string.clear();
            uint32_t _size25;
            ::apache::thrift::protocol::TType _etype28;
            xfer += iprot->readListBegin(_etype28, _size25);
            this->_list_string.resize(_size25);
            uint32_t _i29;
            for (_i29 = 0; _i29 < _size25; ++_i29)
            {
              xfer += iprot->readString(this->_list_string[_i29]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset._list_string = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 13:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->_binary);
          this->__isset._binary = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 14:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->_payload);
          this->__isset._payload = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset__timestamp)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset__sender_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Message::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Message");

  xfer += oprot->writeFieldBegin("_timestamp", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->_timestamp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("_sender_id", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->_sender_id);
  xfer += oprot->writeFieldEnd();

  if (this->__isset._receiver_id) {
    xfer += oprot->writeFieldBegin("_receiver_id", ::apache::thrift::protocol::T_STRING, 3);
    xfer += oprot->writeString(this->_receiver_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._subject) {
    xfer += oprot->writeFieldBegin("_subject", ::apache::thrift::protocol::T_STRING, 4);
    xfer += oprot->writeString(this->_subject);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._sequence_no) {
    xfer += oprot->writeFieldBegin("_sequence_no", ::apache::thrift::protocol::T_I64, 5);
    xfer += oprot->writeI64(this->_sequence_no);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._total_count) {
    xfer += oprot->writeFieldBegin("_total_count", ::apache::thrift::protocol::T_I64, 6);
    xfer += oprot->writeI64(this->_total_count);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_bool) {
    xfer += oprot->writeFieldBegin("_list_bool", ::apache::thrift::protocol::T_LIST, 7);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_BOOL, static_cast<uint32_t>(this->_list_bool.size()));
      std::vector<bool> ::const_iterator _iter30;
      for (_iter30 = this->_list_bool.begin(); _iter30 != this->_list_bool.end(); ++_iter30)
      {
        xfer += oprot->writeBool((*_iter30));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_i16) {
    xfer += oprot->writeFieldBegin("_list_i16", ::apache::thrift::protocol::T_LIST, 8);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I16, static_cast<uint32_t>(this->_list_i16.size()));
      std::vector<int16_t> ::const_iterator _iter31;
      for (_iter31 = this->_list_i16.begin(); _iter31 != this->_list_i16.end(); ++_iter31)
      {
        xfer += oprot->writeI16((*_iter31));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_i32) {
    xfer += oprot->writeFieldBegin("_list_i32", ::apache::thrift::protocol::T_LIST, 9);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I32, static_cast<uint32_t>(this->_list_i32.size()));
      std::vector<int32_t> ::const_iterator _iter32;
      for (_iter32 = this->_list_i32.begin(); _iter32 != this->_list_i32.end(); ++_iter32)
      {
        xfer += oprot->writeI32((*_iter32));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_i64) {
    xfer += oprot->writeFieldBegin("_list_i64", ::apache::thrift::protocol::T_LIST, 10);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I64, static_cast<uint32_t>(this->_list_i64.size()));
      std::vector<int64_t> ::const_iterator _iter33;
      for (_iter33 = this->_list_i64.begin(); _iter33 != this->_list_i64.end(); ++_iter33)
      {
        xfer += oprot->writeI64((*_iter33));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_double) {
    xfer += oprot->writeFieldBegin("_list_double", ::apache::thrift::protocol::T_LIST, 11);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_DOUBLE, static_cast<uint32_t>(this->_list_double.size()));
      std::vector<double> ::const_iterator _iter34;
      for (_iter34 = this->_list_double.begin(); _iter34 != this->_list_double.end(); ++_iter34)
      {
        xfer += oprot->writeDouble((*_iter34));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._list_string) {
    xfer += oprot->writeFieldBegin("_list_string", ::apache::thrift::protocol::T_LIST, 12);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->_list_string.size()));
      std::vector<std::string> ::const_iterator _iter35;
      for (_iter35 = this->_list_string.begin(); _iter35 != this->_list_string.end(); ++_iter35)
      {
        xfer += oprot->writeString((*_iter35));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._binary) {
    xfer += oprot->writeFieldBegin("_binary", ::apache::thrift::protocol::T_STRING, 13);
    xfer += oprot->writeBinary(this->_binary);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset._payload) {
    xfer += oprot->writeFieldBegin("_payload", ::apache::thrift::protocol::T_STRING, 14);
    xfer += oprot->writeString(this->_payload);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Message &a, Message &b) {
  using ::std::swap;
  swap(a._timestamp, b._timestamp);
  swap(a._sender_id, b._sender_id);
  swap(a._receiver_id, b._receiver_id);
  swap(a._subject, b._subject);
  swap(a._sequence_no, b._sequence_no);
  swap(a._total_count, b._total_count);
  swap(a._list_bool, b._list_bool);
  swap(a._list_i16, b._list_i16);
  swap(a._list_i32, b._list_i32);
  swap(a._list_i64, b._list_i64);
  swap(a._list_double, b._list_double);
  swap(a._list_string, b._list_string);
  swap(a._binary, b._binary);
  swap(a._payload, b._payload);
  swap(a.__isset, b.__isset);
}

Message::Message(const Message& other36) {
  _timestamp = other36._timestamp;
  _sender_id = other36._sender_id;
  _receiver_id = other36._receiver_id;
  _subject = other36._subject;
  _sequence_no = other36._sequence_no;
  _total_count = other36._total_count;
  _list_bool = other36._list_bool;
  _list_i16 = other36._list_i16;
  _list_i32 = other36._list_i32;
  _list_i64 = other36._list_i64;
  _list_double = other36._list_double;
  _list_string = other36._list_string;
  _binary = other36._binary;
  _payload = other36._payload;
  __isset = other36.__isset;
}
Message& Message::operator=(const Message& other37) {
  _timestamp = other37._timestamp;
  _sender_id = other37._sender_id;
  _receiver_id = other37._receiver_id;
  _subject = other37._subject;
  _sequence_no = other37._sequence_no;
  _total_count = other37._total_count;
  _list_bool = other37._list_bool;
  _list_i16 = other37._list_i16;
  _list_i32 = other37._list_i32;
  _list_i64 = other37._list_i64;
  _list_double = other37._list_double;
  _list_string = other37._list_string;
  _binary = other37._binary;
  _payload = other37._payload;
  __isset = other37.__isset;
  return *this;
}
void Message::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Message(";
  out << "_timestamp=" << to_string(_timestamp);
  out << ", " << "_sender_id=" << to_string(_sender_id);
  out << ", " << "_receiver_id="; (__isset._receiver_id ? (out << to_string(_receiver_id)) : (out << "<null>"));
  out << ", " << "_subject="; (__isset._subject ? (out << to_string(_subject)) : (out << "<null>"));
  out << ", " << "_sequence_no="; (__isset._sequence_no ? (out << to_string(_sequence_no)) : (out << "<null>"));
  out << ", " << "_total_count="; (__isset._total_count ? (out << to_string(_total_count)) : (out << "<null>"));
  out << ", " << "_list_bool="; (__isset._list_bool ? (out << to_string(_list_bool)) : (out << "<null>"));
  out << ", " << "_list_i16="; (__isset._list_i16 ? (out << to_string(_list_i16)) : (out << "<null>"));
  out << ", " << "_list_i32="; (__isset._list_i32 ? (out << to_string(_list_i32)) : (out << "<null>"));
  out << ", " << "_list_i64="; (__isset._list_i64 ? (out << to_string(_list_i64)) : (out << "<null>"));
  out << ", " << "_list_double="; (__isset._list_double ? (out << to_string(_list_double)) : (out << "<null>"));
  out << ", " << "_list_string="; (__isset._list_string ? (out << to_string(_list_string)) : (out << "<null>"));
  out << ", " << "_binary="; (__isset._binary ? (out << to_string(_binary)) : (out << "<null>"));
  out << ", " << "_payload="; (__isset._payload ? (out << to_string(_payload)) : (out << "<null>"));
  out << ")";
}

} // namespace