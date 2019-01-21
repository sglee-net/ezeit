namespace cpp thrift_gen
namespace java org.chronotics.talaria.thrift.gen
namespace php thrift_gen 

struct ThriftMessage {
	1:  required string 			_sender_id;
	2:  optional string 			_receiver_id;
	3:  optional string			_timestamp;
	4:  optional string 			_subject;
	5:  optional i64 			_sequence_no;
	6:  optional i64			_total_count;
	7:  optional list<bool> 		_list_bool;
	8:  optional list<i16> 			_list_i16;
	9:  optional list<i32> 			_list_i32;
	10: optional list<i64> 			_list_i64;
	11: optional list<double> 		_list_double;
	12: optional list<string>		_list_string;
	13: optional binary 			_binary;
	14: optional string 			_payload;
	15: optional list<ThriftMessage> 	_list_message;
	16: optional set<ThriftMessage>		_set_message;
	17: optional map<string,ThriftMessage> 	_map_message;
}

exception InvalidOperationException {
	1: i32 code,
	2: string description
}

service ThriftRWService {
	bool ping()
		throws(1:InvalidOperationException e),
	string writeThriftMessage(1:ThriftMessage _v) 
		throws(1:InvalidOperationException e),
	string writeBool(1:string _id, 2:bool _v)
		throws(1:InvalidOperationException e),
	string writeI16(1:string _id, 2:i16 _v)
		throws(1:InvalidOperationException e),
	string writeI32(1:string _id, 2:i32 _v)
		throws(1:InvalidOperationException e),
	string writeI64(1:string _id, 2:i64 _v)
		throws(1:InvalidOperationException e),
	string writeDouble(1:string _id, 2:double _v)
		throws(1:InvalidOperationException e),
	string writeString(1:string _id, 2:string _v)
		throws(1:InvalidOperationException e),
	ThriftMessage readThriftMessage(1:string _id)
		throws(1:InvalidOperationException e),
	bool readBool(1:string _id)
		throws(1:InvalidOperationException e),
	i16 readI16(1:string _id)
		throws(1:InvalidOperationException e),
	i32 readI32(1:string _id)
		throws(1:InvalidOperationException e),
	i64 readI64(1:string _id)
		throws(1:InvalidOperationException e),
	double readDouble(1:string _id)
		throws(1:InvalidOperationException e),
	string readString(1:string _id)
		throws(1:InvalidOperationException e),
	bool writeId(1:string _id)
		throws(1:InvalidOperationException e),
	list<string> readId()
		throws(1:InvalidOperationException e)
}
