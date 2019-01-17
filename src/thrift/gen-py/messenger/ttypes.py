#
# Autogenerated by Thrift Compiler (1.0.0-dev)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TFrozenDict, TException, TApplicationException
from thrift.protocol.TProtocol import TProtocolException
from thrift.TRecursive import fix_spec

import sys

from thrift.transport import TTransport
all_structs = []


class Message(object):
    """
    Attributes:
     - sender_id
     - receiver_id
     - timestamp
     - subject
     - sequence_no
     - total_count
     - list_bool
     - list_i16
     - list_i32
     - list_i64
     - list_double
     - list_string
     - binary_value
     - payload
     - list_message
     - set_message
     - map_message

    """


    def __init__(self, sender_id=None, receiver_id=None, timestamp=None, subject=None, sequence_no=None, total_count=None, list_bool=None, list_i16=None, list_i32=None, list_i64=None, list_double=None, list_string=None, binary_value=None, payload=None, list_message=None, set_message=None, map_message=None,):
        self.sender_id = sender_id
        self.receiver_id = receiver_id
        self.timestamp = timestamp
        self.subject = subject
        self.sequence_no = sequence_no
        self.total_count = total_count
        self.list_bool = list_bool
        self.list_i16 = list_i16
        self.list_i32 = list_i32
        self.list_i64 = list_i64
        self.list_double = list_double
        self.list_string = list_string
        self.binary_value = binary_value
        self.payload = payload
        self.list_message = list_message
        self.set_message = set_message
        self.map_message = map_message

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.sender_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.STRING:
                    self.receiver_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 3:
                if ftype == TType.STRING:
                    self.timestamp = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 4:
                if ftype == TType.STRING:
                    self.subject = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 5:
                if ftype == TType.I64:
                    self.sequence_no = iprot.readI64()
                else:
                    iprot.skip(ftype)
            elif fid == 6:
                if ftype == TType.I64:
                    self.total_count = iprot.readI64()
                else:
                    iprot.skip(ftype)
            elif fid == 7:
                if ftype == TType.LIST:
                    self.list_bool = []
                    (_etype3, _size0) = iprot.readListBegin()
                    for _i4 in range(_size0):
                        _elem5 = iprot.readBool()
                        self.list_bool.append(_elem5)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 8:
                if ftype == TType.LIST:
                    self.list_i16 = []
                    (_etype9, _size6) = iprot.readListBegin()
                    for _i10 in range(_size6):
                        _elem11 = iprot.readI16()
                        self.list_i16.append(_elem11)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 9:
                if ftype == TType.LIST:
                    self.list_i32 = []
                    (_etype15, _size12) = iprot.readListBegin()
                    for _i16 in range(_size12):
                        _elem17 = iprot.readI32()
                        self.list_i32.append(_elem17)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 10:
                if ftype == TType.LIST:
                    self.list_i64 = []
                    (_etype21, _size18) = iprot.readListBegin()
                    for _i22 in range(_size18):
                        _elem23 = iprot.readI64()
                        self.list_i64.append(_elem23)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 11:
                if ftype == TType.LIST:
                    self.list_double = []
                    (_etype27, _size24) = iprot.readListBegin()
                    for _i28 in range(_size24):
                        _elem29 = iprot.readDouble()
                        self.list_double.append(_elem29)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 12:
                if ftype == TType.LIST:
                    self.list_string = []
                    (_etype33, _size30) = iprot.readListBegin()
                    for _i34 in range(_size30):
                        _elem35 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                        self.list_string.append(_elem35)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 13:
                if ftype == TType.STRING:
                    self.binary_value = iprot.readBinary()
                else:
                    iprot.skip(ftype)
            elif fid == 14:
                if ftype == TType.STRING:
                    self.payload = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 15:
                if ftype == TType.LIST:
                    self.list_message = []
                    (_etype39, _size36) = iprot.readListBegin()
                    for _i40 in range(_size36):
                        _elem41 = Message()
                        _elem41.read(iprot)
                        self.list_message.append(_elem41)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 16:
                if ftype == TType.SET:
                    self.set_message = set()
                    (_etype45, _size42) = iprot.readSetBegin()
                    for _i46 in range(_size42):
                        _elem47 = Message()
                        _elem47.read(iprot)
                        self.set_message.add(_elem47)
                    iprot.readSetEnd()
                else:
                    iprot.skip(ftype)
            elif fid == 17:
                if ftype == TType.MAP:
                    self.map_message = {}
                    (_ktype49, _vtype50, _size48) = iprot.readMapBegin()
                    for _i52 in range(_size48):
                        _key53 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                        _val54 = Message()
                        _val54.read(iprot)
                        self.map_message[_key53] = _val54
                    iprot.readMapEnd()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('Message')
        if self.sender_id is not None:
            oprot.writeFieldBegin('sender_id', TType.STRING, 1)
            oprot.writeString(self.sender_id.encode('utf-8') if sys.version_info[0] == 2 else self.sender_id)
            oprot.writeFieldEnd()
        if self.receiver_id is not None:
            oprot.writeFieldBegin('receiver_id', TType.STRING, 2)
            oprot.writeString(self.receiver_id.encode('utf-8') if sys.version_info[0] == 2 else self.receiver_id)
            oprot.writeFieldEnd()
        if self.timestamp is not None:
            oprot.writeFieldBegin('timestamp', TType.STRING, 3)
            oprot.writeString(self.timestamp.encode('utf-8') if sys.version_info[0] == 2 else self.timestamp)
            oprot.writeFieldEnd()
        if self.subject is not None:
            oprot.writeFieldBegin('subject', TType.STRING, 4)
            oprot.writeString(self.subject.encode('utf-8') if sys.version_info[0] == 2 else self.subject)
            oprot.writeFieldEnd()
        if self.sequence_no is not None:
            oprot.writeFieldBegin('sequence_no', TType.I64, 5)
            oprot.writeI64(self.sequence_no)
            oprot.writeFieldEnd()
        if self.total_count is not None:
            oprot.writeFieldBegin('total_count', TType.I64, 6)
            oprot.writeI64(self.total_count)
            oprot.writeFieldEnd()
        if self.list_bool is not None:
            oprot.writeFieldBegin('list_bool', TType.LIST, 7)
            oprot.writeListBegin(TType.BOOL, len(self.list_bool))
            for iter55 in self.list_bool:
                oprot.writeBool(iter55)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.list_i16 is not None:
            oprot.writeFieldBegin('list_i16', TType.LIST, 8)
            oprot.writeListBegin(TType.I16, len(self.list_i16))
            for iter56 in self.list_i16:
                oprot.writeI16(iter56)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.list_i32 is not None:
            oprot.writeFieldBegin('list_i32', TType.LIST, 9)
            oprot.writeListBegin(TType.I32, len(self.list_i32))
            for iter57 in self.list_i32:
                oprot.writeI32(iter57)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.list_i64 is not None:
            oprot.writeFieldBegin('list_i64', TType.LIST, 10)
            oprot.writeListBegin(TType.I64, len(self.list_i64))
            for iter58 in self.list_i64:
                oprot.writeI64(iter58)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.list_double is not None:
            oprot.writeFieldBegin('list_double', TType.LIST, 11)
            oprot.writeListBegin(TType.DOUBLE, len(self.list_double))
            for iter59 in self.list_double:
                oprot.writeDouble(iter59)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.list_string is not None:
            oprot.writeFieldBegin('list_string', TType.LIST, 12)
            oprot.writeListBegin(TType.STRING, len(self.list_string))
            for iter60 in self.list_string:
                oprot.writeString(iter60.encode('utf-8') if sys.version_info[0] == 2 else iter60)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.binary_value is not None:
            oprot.writeFieldBegin('binary_value', TType.STRING, 13)
            oprot.writeBinary(self.binary_value)
            oprot.writeFieldEnd()
        if self.payload is not None:
            oprot.writeFieldBegin('payload', TType.STRING, 14)
            oprot.writeString(self.payload.encode('utf-8') if sys.version_info[0] == 2 else self.payload)
            oprot.writeFieldEnd()
        if self.list_message is not None:
            oprot.writeFieldBegin('list_message', TType.LIST, 15)
            oprot.writeListBegin(TType.STRUCT, len(self.list_message))
            for iter61 in self.list_message:
                iter61.write(oprot)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        if self.set_message is not None:
            oprot.writeFieldBegin('set_message', TType.SET, 16)
            oprot.writeSetBegin(TType.STRUCT, len(self.set_message))
            for iter62 in self.set_message:
                iter62.write(oprot)
            oprot.writeSetEnd()
            oprot.writeFieldEnd()
        if self.map_message is not None:
            oprot.writeFieldBegin('map_message', TType.MAP, 17)
            oprot.writeMapBegin(TType.STRING, TType.STRUCT, len(self.map_message))
            for kiter63, viter64 in self.map_message.items():
                oprot.writeString(kiter63.encode('utf-8') if sys.version_info[0] == 2 else kiter63)
                viter64.write(oprot)
            oprot.writeMapEnd()
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        if self.sender_id is None:
            raise TProtocolException(message='Required field sender_id is unset!')
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class InvalidOperationException(TException):
    """
    Attributes:
     - code
     - description

    """


    def __init__(self, code=None, description=None,):
        self.code = code
        self.description = description

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.I32:
                    self.code = iprot.readI32()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.STRING:
                    self.description = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('InvalidOperationException')
        if self.code is not None:
            oprot.writeFieldBegin('code', TType.I32, 1)
            oprot.writeI32(self.code)
            oprot.writeFieldEnd()
        if self.description is not None:
            oprot.writeFieldBegin('description', TType.STRING, 2)
            oprot.writeString(self.description.encode('utf-8') if sys.version_info[0] == 2 else self.description)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __str__(self):
        return repr(self)

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(Message)
Message.thrift_spec = (
    None,  # 0
    (1, TType.STRING, 'sender_id', 'UTF8', None, ),  # 1
    (2, TType.STRING, 'receiver_id', 'UTF8', None, ),  # 2
    (3, TType.STRING, 'timestamp', 'UTF8', None, ),  # 3
    (4, TType.STRING, 'subject', 'UTF8', None, ),  # 4
    (5, TType.I64, 'sequence_no', None, None, ),  # 5
    (6, TType.I64, 'total_count', None, None, ),  # 6
    (7, TType.LIST, 'list_bool', (TType.BOOL, None, False), None, ),  # 7
    (8, TType.LIST, 'list_i16', (TType.I16, None, False), None, ),  # 8
    (9, TType.LIST, 'list_i32', (TType.I32, None, False), None, ),  # 9
    (10, TType.LIST, 'list_i64', (TType.I64, None, False), None, ),  # 10
    (11, TType.LIST, 'list_double', (TType.DOUBLE, None, False), None, ),  # 11
    (12, TType.LIST, 'list_string', (TType.STRING, 'UTF8', False), None, ),  # 12
    (13, TType.STRING, 'binary_value', 'BINARY', None, ),  # 13
    (14, TType.STRING, 'payload', 'UTF8', None, ),  # 14
    (15, TType.LIST, 'list_message', (TType.STRUCT, [Message, None], False), None, ),  # 15
    (16, TType.SET, 'set_message', (TType.STRUCT, [Message, None], False), None, ),  # 16
    (17, TType.MAP, 'map_message', (TType.STRING, 'UTF8', TType.STRUCT, [Message, None], False), None, ),  # 17
)
all_structs.append(InvalidOperationException)
InvalidOperationException.thrift_spec = (
    None,  # 0
    (1, TType.I32, 'code', None, None, ),  # 1
    (2, TType.STRING, 'description', 'UTF8', None, ),  # 2
)
fix_spec(all_structs)
del all_structs
