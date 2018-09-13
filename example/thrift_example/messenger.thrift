namespace cpp thrift_gen_messenger
namespace java org.chronotics.talaria.thrift.gen
namespace php thrift_gen_messenger

struct Message {
	1:  required string 		_timestamp;
	2:  required string 		_sender_id;
	3:  optional string 		_receiver_id;
	4:  optional string 		_subject;
	5:  optional i64 		_sequence_no;
	6:  optional i64		_total_count;
	7:  optional list<bool> 	_list_bool;
	8:  optional list<i16> 		_list_i16;
	9:  optional list<i32> 		_list_i32;
	10: optional list<i64> 		_list_i64;
	11: optional list<double> 	_list_double;
	12: optional list<string>	_list_string;
	13: optional binary 		_binary;
	14: optional string 		_payload;
}

service TransferService {
	string writeMessage(1:Message _v),
	string writeBool(1:string _id, 2:bool _v),
	string writeI16(1:string _id, 2:i16 _v),
	string writeI32(1:string _id, 2:i32 _v),
	string writeI64(1:string _id, 2:i64 _v),
	string writeDouble(1:string _id, 2:double _v),
	string writeString(1:string _id, 2:string _v),
	Message readMessage(1:string _id),
	bool readBool(1:string _id),
	i16 readI16(1:string _id),
	i32 readI32(1:string _id),
	i64 readI64(1:string _id),
	double readDouble(1:string _id),
	string readString(1:string _id),
	bool writeId(1:string _id),
	list<string> readId()
}
