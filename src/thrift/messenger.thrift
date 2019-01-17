namespace cpp thrift_gen
namespace java org.chronotics.talaria.thrift_gen
namespace php thrift_gen 

struct Message {
	1:  required string 			sender_id;
	2:  optional string 			receiver_id;
	3:  optional string			timestamp;
	4:  optional string 			subject;
	5:  optional i64 			sequence_no;
	6:  optional i64			total_count;
	7:  optional list<bool> 		list_bool;
	8:  optional list<i16> 			list_i16;
	9:  optional list<i32> 			list_i32;
	10: optional list<i64> 			list_i64;
	11: optional list<double> 		list_double;
	12: optional list<string>		list_string;
	13: optional binary 			binary_value;
	14: optional string 			payload;
	15: optional list<Message> 		list_message;
	16: optional set<Message>		set_message;
	17: optional map<string,Message> 	map_message;
}

exception InvalidOperationException {
	1: i32 code,
	2: string description
}

service ThriftService {
	bool ping()
		throws(1:InvalidOperationException e),
	string writeMessage(1:Message _v) 
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
	Message readMessage(1:string _id)
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
