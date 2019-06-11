// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>  
#include <regex>

using namespace std;
#define _debug_test_
#ifdef _debug_test_

/* MCC-MNC-Digit */
typedef long	 MCC_MNC_Digit_t;
#define	A_SET_OF(type)					\
	struct {					\
		type **array;				\
		int count;	/* Meaningful size */	\
		int size;	/* Allocated size */	\
		void (*free)(type *);			\
	}

#define	A_SEQUENCE_OF(type)	A_SET_OF(type)

typedef struct MCC {
	A_SEQUENCE_OF(MCC_MNC_Digit_t) list;

} MCC_t;
typedef size_t ber_tlv_len_t;
typedef struct asn_struct_ctx_s {
	short phase;		/* Decoding phase */
	short step;		/* Elementary step of a phase */
	int context;		/* Other context information */
	void *ptr;		/* Decoder-specific stuff (stack elements) */
	ber_tlv_len_t left;	/* Number of bytes left, -1 for indefinite */
} asn_struct_ctx_t;
#endif
/**/
//#define offsetof(s,m) ((size_t)&(((s*)0)->m))
#define getmem(s,m) (((s*)0)->m)
typedef struct member_s {
	int a;
	char b;
	long c;
}member_t;
typedef struct asn_codec_ctx_s {
	size_t  max_stack_size; /* 0 disables stack bounds checking */
} asn_codec_ctx_t;
static int ASN__STACK_OVERFLOW_CHECK(asn_codec_ctx_t *ctx) {
	if (ctx /*&& ctx->max_stack_size*/) {

		/* ctx MUST be allocated on the stack */
		int c = (int)(char*)ctx;
		int d = (int)(char *)&ctx;
		

		ptrdiff_t usedstack = ((char *)ctx - (char *)&ctx);
		if (usedstack > 0) usedstack = -usedstack; /* grows up! */

		/* double negative required to avoid int wrap-around */
		/*if (usedstack < -(ptrdiff_t)ctx->max_stack_size) {
			printf("Stack limit %ld reached",
				(long)ctx->max_stack_size);
			return -1;
		}*/
	}
	return 0;
}
const int  BASE(0);
const int  EUTRA_RRC_Definitions(0);
const int  PC5_RRC_Definitions(1);
const int  NBIOT_RRC_Definitions(2);
const int  EUTRA_UE_Variables(3);
const int  NBIOT_UE_Variables(4);
const int  EUTRA_Sidelink_Preconf(5);
const int  EUTRA_InterNodeDefinitions(6);
const int  NBIOT_InterNodeDefinitions(7);
const short status =	(1 << EUTRA_RRC_Definitions) | \
						(1 << PC5_RRC_Definitions) | \
						(1 << NBIOT_RRC_Definitions) | \
						(1 << EUTRA_UE_Variables) | \
						(1 << NBIOT_UE_Variables) | \
						(1 << EUTRA_Sidelink_Preconf) | \
						(1 << EUTRA_InterNodeDefinitions) | \
						(1 << NBIOT_InterNodeDefinitions);

const std::string EUTRA_RRC("EUTRA-RRC-Definitions");
const std::string PC5_RRC("PC5-RRC-Definitions");
const std::string NBIOT_RRC("NBIOT-RRC-Definitions");
const std::string EUTRA_UE("EUTRA-UE-Variables");
const std::string NBIOT_UE("NBIOT-UE-Variables");
const std::string EUTRA_Sidelink("EUTRA-Sidelink-Preconf");
const std::string EUTRA_InterNode("EUTRA-InterNodeDefinitions");
const std::string NBIOT_InterNode("NBIOT-InterNodeDefinitions");


std::vector<std::string> vec_asn_global;

std::vector<std::string> vec_asn_EUTRA_RRC;
std::vector<std::string> vec_asn_PC5_RRC;
std::vector<std::string> vec_asn_NBIOT_RRC;
std::vector<std::string> vec_asn_EUTRA_UE;
std::vector<std::string> vec_asn_NBIOT_UE;
std::vector<std::string> vec_asn_EUTRA_Sidelink;
std::vector<std::string> vec_asn_EUTRA_InterNode;
std::vector<std::string> vec_asn_NBIOT_InterNode;

std::vector<std::string> vec_title_str;
std::vector<std::string> vec_outermost_str;// 最外层的asn :: SEQUENCE{...}
std::vector<std::string> vec_body_str; //仅仅存储body内容，
std::vector<std::string> vec_body_next_tag_str; //子信元类型对象

std::map<std::string, std::vector<std::string>&> module_map;



//module_map.insert(pair<int, std::vector<std::string>>(EUTRA_RRC_Definitions, vec_asn_EUTRA_RRC);
std::string output_file;
std::string input_file = "D:/3gpp/36331-ea0/36331-ea0.txt";
bool startFlg(false);
bool endFlg(false);
std::vector<std::string> vec_basic_type;
std::vector<std::string> vec_composite_type;
std::vector<std::string> vec_import_asn;

std::string moduleName;//导入的模块名称
const int continu = 0;//分析body中的每一行，找到需要继续递归search的tag
int body_lbrace = 0;//分析body中的每一行，body中的左'{'
int body_rbrace = 0;//分析body中的每一行，body中的左'}'

int next_tag = 0; //每行只有变量名，没有值对象，需要查找下面一行
std::string next_tag_var;

static int segment_str(std::string& lineStr) {
	regex patternHeader1("(\\s*)([0-9A-Za-z\\-_\\.]*)(\\s*)([0-9A-Za-z\\-_\\.]*)(\\s*)([\\{]*)");
	std::string varible = regex_replace(lineStr, patternHeader1, std::string("$2"));//以括号为组，变量
	std::string value = regex_replace(lineStr, patternHeader1, std::string("$4"));//值对象
	
	if (next_tag) {
		regex patternHeader2("(\\s*)([0-9A-Za-z\\-_\\.]*)(,)(\\s*)");
		std::string subObject = regex_replace(lineStr, patternHeader2, std::string("$2"));
		if (!subObject.empty()) { //如果不为空，暂且加入到vec中，待后续递归查找
			vec_body_next_tag_str.push_back(subObject);
			return 1;
		}
	}
	/* 如果值为空，则继续找下一行 */
	if (value.empty() && !next_tag) {
		next_tag = 1;
		return(-1);
	}
	
	else {
		vec_body_str.push_back(value);
	}
	return 0;
}

static void product_file(std::string& moduleName, std::vector<std::string> *vecAction) {
	std::vector<std::string >::iterator itr;
	int pos = input_file.find('.');
	if (pos == std::string::npos) {
		output_file = input_file + "-" + moduleName + ".asn"; //!!
	}
	else {
		output_file = input_file.substr(0, pos) + "-" + moduleName + ".asn";
		std::cout << output_file << std::endl;
	}
	std::fstream output;
	output.open(output_file.c_str(), std::fstream::out);
	if (output.fail() == true) {
		std::cout << "The output file can not be created here !" << std::endl;
		return ;
	}
	else {
		std::cout << "output file path:" << output_file.c_str() << std::endl;
	}

	for (itr = vecAction->begin(); itr != vecAction->end(); ++itr) {
		output << *itr << std::endl;
	}
	output.close();
}
static int sort_module() {
	
	std::map<std::string, std::vector<std::string>>::iterator  itrMap;

	std::string moduleName;
	std::vector<std::string >::iterator itr;
	std::map<std::string, std::vector<std::string>>::iterator it;
	
	std::vector<std::string> *vecAction = nullptr;
	std::string tmpStr;
	std::string subString;
	for (itr = vec_asn_global.begin(); itr != vec_asn_global.end(); ++itr) {
		tmpStr = *itr;
		int ret(-1);
		if ((!startFlg) && ((ret = tmpStr.find("DEFINITIONS AUTOMATIC TAGS")) != std::string::npos)) {
			subString = tmpStr.substr(0, ret);
			moduleName = subString.substr(subString.find_first_not_of(" "), subString.find_last_of(" ") - subString.find_first_not_of(" "));

			vecAction = &(module_map.find(moduleName)->second);
			startFlg = true;
			endFlg = false;
			vecAction->push_back(tmpStr);
		}
		else if ((tmpStr.find("END") != std::string::npos) && (!endFlg)) {
			endFlg = true;
			startFlg = false;
			vecAction->push_back(tmpStr);
			product_file(moduleName, vecAction);
		}
		else if (startFlg) {
			vecAction->push_back(tmpStr);
		}
	}
	
	return 0;
}

static int analysis(std::string& analysis_line_str) {
	if (analysis_line_str.find("{") != std::string::npos) {
		body_lbrace++;
		return continu;
	}

	if (analysis_line_str.find("}") != std::string::npos) {
		body_rbrace++;
		return continu;
	}
	(void)segment_str(analysis_line_str);
	return 0;
}
static int brace_count(std::string& str, int& left, int& right) {
	int start = 0;
	int cnt = 0;
	while ((start = str.find('{', start)) != std::string::npos) {
		left++,start++;
	}
	start = 0;
	while ((start = str.find('}', start)) != std::string::npos) {
		right++, start++;
	}
	return (left - right);
}

/*
	找到需要导入的模块名称以及具体需要导入的信元对象，加入到vec_import_asn中
*/
static int find_import_content() {
	
	std::string tmpStr;
	std::string tmpStrHandle;
	std::vector<std::string>::iterator itr;
	bool import_start(false);
	for (itr = vec_asn_NBIOT_RRC.begin(); itr != vec_asn_NBIOT_RRC.end(); ++itr) {
		tmpStr = *itr;
		if (tmpStr.find("IMPORT") != std::string::npos) {
			import_start = true;
			continue;
		}
		else if (tmpStr.find("FROM") != std::string::npos) {
			int idx = tmpStr.find(" ");
			moduleName = tmpStr.substr(idx + 1, tmpStr.length() - idx - 2);
			//vecAction = &(module_map.find(moduleName)->second);//找到IMPORT ...FROM 具体模块
			break;
		}
		else if (import_start && tmpStr.size()) {
			tmpStrHandle = tmpStr.substr(tmpStr.find_first_not_of("\\\t"), tmpStr.find_first_of(",") - 1);//去除不需要的字符
			vec_import_asn.push_back(tmpStrHandle);//找到需要导入的asn组 e.g. RRCConnectionReestablishmentReject
		}
	}
	return 0;
}
/*
	从需要导入的vec中遍历
	RRCConnectionReestablishmentReject ::= SEQUENCE {
			criticalExtensions					CHOICE {
			rrcConnectionReestablishmentReject-r8
										RRCConnectionReestablishmentReject-r8-IEs,
			criticalExtensionsFuture			SEQUENCE {}
		}
	}
*/

static void parse_vec_asn_NBIOT_RRC() {
	//vec_asn_NBIOT_RRC
	std::vector<std::string>::iterator subItr;
	std::vector<std::string>::iterator importItr;
	std::map<std::string, std::vector<std::string>>::iterator it;
	std::vector<std::string> *vecAction = nullptr; 

	std::string tmpStr;
	
	vecAction = &(module_map.find(moduleName)->second);//找到IMPORT ...FROM 具体模块
	int ret = 0;
	int left_brace = 0;//左大括号
	int right_brace = 0;//右大括号
	bool startExtractFlg(false);
	
	std::string import_line_module;// 导入的每行RRCConnectionReestablishmentReject模块名
	for (importItr = vec_import_asn.begin(); importItr != vec_import_asn.end(); ++importItr) {
		import_line_module = *importItr;
		
		/* 正则匹配最外层头和尾 */
		regex patternHeader("(\\s*)" + import_line_module + "(\\s*)(::=)(\\s+)([0-9A-Za-z\\-_\\.]+)(\\s+)(\\{)");
		regex patternTail("(\\s*)(\\})(\\s*)");
		for (subItr = vecAction->begin(); subItr != vecAction->end(); ++subItr) {
			tmpStr = *subItr;
			int cmp = brace_count(tmpStr, left_brace, right_brace);//比较'{'和'}'个数
			if (regex_match(tmpStr, patternHeader) && !startExtractFlg){
				startExtractFlg = true;
				vec_outermost_str.push_back(tmpStr);
			}
			else if (startExtractFlg) {
				ret = analysis(tmpStr);// 分析内部的信元对象是基本类型、复合类型还是存在其它子类型
				vec_outermost_str.push_back(tmpStr);
				if (regex_match(tmpStr, patternTail) && !cmp) {
					startExtractFlg = false;//最外层的asn结束,接着递归里面的信元对象
					break;
				}
			}
		}
	}
	
	/*std::fstream output;
	output.open("D:\\3gpp\\36331-ea0\\test.asn", std::fstream::out);
	for (subItr = vecAction->begin(); subItr != vecAction->end(); ++subItr) {
		output << *subItr << std::endl;
	}
	output.close();*/
	
}

int main() {

	/*asn_codec_ctx_t t;
	t.max_stack_size = 30000;
	int a1 = 4;
	int *p = &a1;
	int pv = (int)&p;
	ASN__STACK_OVERFLOW_CHECK(&t);

	printf("$$$$$$$$$$$%ld,%ld\n", sizeof(size_t), sizeof(size_t));
	printf("sizeof int=%ld,long=%ld,long long=%ld,double=%ld, float=%ld,void*=%ld\n", sizeof(int), sizeof(long), sizeof(long  long), sizeof(double), sizeof(float), sizeof(void*));
	printf("%ld,%ld,%ld,%ld,%ld\n",sizeof(short),sizeof(int),sizeof(void*),sizeof(ber_tlv_len_t));
	int size = sizeof(asn_struct_ctx_t);
	member_t f_member{ 23,'b',655360 };
	
	int d = offsetof(struct member_s, b);*/
#ifndef _debug_test_
	MCC_t Local_MCC_pt;
	MCC_MNC_Digit_t **local_MCC_Digit_t;
	int MCC_SIZE = 3;
	Local_MCC_pt.list.array = (MCC_MNC_Digit_t **)calloc(MCC_SIZE, sizeof(MCC_MNC_Digit_t *));
	MCC_MNC_Digit_t mcc_fisrt = 0x00;
	MCC_MNC_Digit_t mcc_sec = 0x00;
	MCC_MNC_Digit_t mcc_third = 0x01;

	Local_MCC_pt.list.array[0] = &mcc_fisrt;
	Local_MCC_pt.list.array[1] = &mcc_sec;
	Local_MCC_pt.list.array[2] = &mcc_third;

	Local_MCC_pt.list.count = 3; /*  MCC:460(SIZE3) */
	Local_MCC_pt.list.size = 3;
	

#endif
#ifdef _debug_test_
	
	/*
		generate c files by asn1c.exe
		system command: 
			--linux: asn1c  -S /usr/local/share/asn1c -fcompound-names -fskeletons-copy -gen-PER -pdu=auto 36331-c00.asn
			--window: asn1c -S ../skeletons -f compound-names -fskeletons-copy -gen-PER -pdu=auto  RetangleTest.asn1
	*/
	/* = {
	{EUTRA_RRC_Definitions , vec_asn_EUTRA_RRC},
	{PC5_RRC_Definitions ,vec_asn_PC5_RRC},
	{NBIOT_RRC_Definitions, vec_asn_NBIOT_RRC},
	{EUTRA_UE_Variables, vec_asn_EUTRA_UE},
	{NBIOT_UE_Variables,vec_asn_NBIOT_UE},
	{EUTRA_Sidelink_Preconf,vec_asn_EUTRA_Sidelink},
	{EUTRA_InterNodeDefinitions,vec_asn_EUTRA_InterNode},
	{NBIOT_InterNodeDefinitions,vec_asn_NBIOT_InterNode}
}*/
	/*std::cout << input_file.c_str() << std::endl;
	std::string tmpStr("  RRCConnectionReestablishmentReject ::= SEQUENCE {");
	std::string email("   yjcdoc@163.com");
	std::string nameMod(" criticalExtensions					CHOICE {");
	regex pattern("([0-9A-Za-z\\-_\\.]+)@(([a-zA-Z0-9\\-])+\\.+[a-z]{2,3}(\\.[a-z]{2})?)");
	
	regex patternHeader("(\\s*)([0-9A-Za-z\\-_\\.]+)(\\s+)([0-9A-Za-z\\-_\\.]+)(\\s+)(\\{)");
	std::string name = regex_replace(nameMod, patternHeader, std::string("$6"));
	segment_str(nameMod);
	std::string test_str("FROM EUTRA-RRC-Definitions;");
	int idx = test_str.find(" ");
	std::string ret = test_str.substr(test_str.find(" ")+1, test_str.length()-idx-2)*/;
	
	std::fstream input;
	std::string input_line;
	const unsigned long cul_asn_idle = 0x0;
	const unsigned long cul_asn_start = 0x1;
	unsigned long asn_state = cul_asn_idle;
	input.open(input_file.c_str(), std::fstream::in);
	if (input.fail() == true) {
		std::cout << "Please check input file is correct !" << std::endl;
		return 1;
	}
	while (std::getline(input, input_line)) {
		if (cul_asn_idle == asn_state) {
			if (input_line.find("-- ASN1START") != std::string::npos) {
				asn_state |= cul_asn_start;
			}
			continue;
		}
		if (0 != (cul_asn_start & asn_state)) {
			if (input_line.find("-- ASN1STOP") != std::string::npos) {
				asn_state = cul_asn_idle;
			}
			else {
				if (input_file.find("DEFINITIONS AUTOMATIC TAGS") != std::string::npos) {
					vec_title_str.push_back(input_file);
					std::cout << input_file << std::endl;
				}
				vec_asn_global.push_back(input_line);
			}
		}
	}
	input.close();
	module_map.insert(pair<std::string, std::vector<std::string>&>(EUTRA_RRC, vec_asn_EUTRA_RRC));
	module_map.insert(pair<std::string, std::vector<std::string>&>(PC5_RRC, vec_asn_PC5_RRC));
	module_map.insert(pair<std::string, std::vector<std::string>&>(NBIOT_RRC, vec_asn_NBIOT_RRC));
	module_map.insert(pair<std::string, std::vector<std::string>&>(EUTRA_UE, vec_asn_EUTRA_UE));
	module_map.insert(pair<std::string, std::vector<std::string>&>(NBIOT_UE, vec_asn_NBIOT_UE));
	module_map.insert(pair<std::string, std::vector<std::string>&>(EUTRA_Sidelink, vec_asn_EUTRA_Sidelink));
	module_map.insert(pair<std::string, std::vector<std::string>&>(EUTRA_InterNode, vec_asn_EUTRA_InterNode));
	module_map.insert(pair<std::string, std::vector<std::string>&>(NBIOT_InterNode, vec_asn_NBIOT_InterNode));
	
	//extract_module(vec_asn_global);
	sort_module();
	vec_basic_type.push_back("BOOL");
	vec_basic_type.push_back("INTEGER");
	vec_basic_type.push_back("REAL");
	vec_basic_type.push_back("BIT STRING");
	vec_basic_type.push_back("OCTET STRING");
	vec_basic_type.push_back("ENUMERATED");
	vec_basic_type.push_back("OBJECT IDENTIFIER");
	vec_basic_type.push_back("NULL");
	vec_composite_type.push_back("SEQENCE");
	vec_composite_type.push_back("SET");
	vec_composite_type.push_back("CHOICE");
	vec_composite_type.push_back("SEQENCE OF");
	vec_composite_type.push_back("SET OF");


	find_import_content();
	parse_vec_asn_NBIOT_RRC();

	/***/
	
#endif
	return 0;
}
#if 0
using namespace std;
int main()
{

	time_t now = time(nullptr);
	struct tm local_tm;
	localtime_s(&local_tm, &now);

	std::cout << (local_tm.tm_year + 1900) << "年" << local_tm.tm_mon << "月" \
		<< local_tm.tm_mday << "日" << "时间:" << local_tm.tm_hour << ":" << local_tm.tm_min << ":" << local_tm.tm_sec << std::endl;
	std::cout << "Hello World!\n";

	test_cl tc;
	tc.data_print();

	std::forward_list<int> fl = { 99,2,3,4,5,8 }; //单项线性链表
	std::cout << "Number: " << fl.max_size() << std::endl;
	for (auto at_fl : fl)
	{
		std::cout << at_fl << std::endl;
	}
	fl.remove(8);
	for (auto at_fl : fl)
	{
		std::cout << at_fl << std::endl;
	}
	fl.clear();
	for (auto at_fl : fl)
	{
		std::cout << "after clear: " << at_fl << std::endl;
	}


	box testbox1;
	box testbox2;
	testbox1.setHigh(1);
	testbox2.setHigh(2);
	testbox1.setLength(3);
	testbox2.setLength(4);
	testbox1.setWidth(5);
	testbox2.setWidth(6);
	std::cout << "box1 = " << testbox1.getArea();
	std::cout << "box2 = " << testbox2.getArea();
	box temp_box;
	temp_box = testbox2 + testbox1;

	std::cout << "box1 plus box2 = " << temp_box.getArea() << "size:" << sizeof(uint64_t);

	uint64_t numerator = 18446744069414584320;// 0xFFFFFFFF00000000;
	uint64_t denominator = 4374732215;// 0x104C11DB7;

	uint64_t remainderY1 = numerator % denominator; //remainder Y1 = 0xC704DD7B


	std::cout << "\nremainderY1: " << hex << std::setiosflags(std::ios::showbase) << remainderY1 << std::endl; // 使用0x十六进制显示
	numerator = 0x1234123400000000;
	uint64_t remainderY2 = numerator % denominator;//remainder Y2 = 0x1D66F1A6
	std::cout << "\nremainderY2: " << hex << remainderY2 << std::endl << std::resetiosflags(std::ios::hex);//最后取消hex的输出方式


	polymorphism_sharp *sharp;
	polymorphism_rectangle rec(10, 4);
	polymorphism_triangle tri(10, 4);

	sharp = &rec;
	sharp->area();

	sharp = &tri;
	sharp->area();
}
#endif


	/*
	
	Inputs:
- Least significant bits of S-TMSI: 0x12341234
- Generator polynomial: 0x104C11DB7 (1 0000 0100 1100 0001 0001 1101 1011 0111) 多项式
Procedure to Calculate Hashed ID:
step a)
- k = 32
- numerator: 0xFFFF FFFF 0000 0000   // 2^32 * (2^31 + 2^30 + 2^29 + 2^28 + ...+ 2^1 + 1) 左移32bit
- denominator: 0x1 04C1 1DB7
- remainder Y1 = 0xC704DD7B
step b)
- numerator: 0x1234 1234 0000 0000   // 2^32 * 0x12341234 左移32bit
- denominator: 0x1 04C1 1DB7
- remainder Y2 = 0x1D66F1A6
Hashed_ID = FCS = ones complement of (remainder Y1 XOR remainder Y2)
= ones complement of (0xC704DD7B XOR 0x1D66F1A6)
= negation of (0xDA622CDD)
= 0x259DD322
	*/




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
