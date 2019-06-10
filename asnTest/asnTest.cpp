// asnTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <stdio.h>
#include "Rectangle.h"

char tab[8];
/*
* This is a custom function which writes the
* encoded output into a global test table
*/

static int decode_callback(const void *buffer, size_t size, void *app_key)
{
	static int i = 0;

	memcpy(&tab[i], buffer, size);

	i += size;

	return i;
}



int main()
{
	Rectangle_t *rectangle; /* Type to encode */
	asn_enc_rval_t ec; /* Encoder return value */

	/* Allocate the Rectangle_t */
	rectangle = (Rectangle_t*)calloc(1, sizeof(Rectangle_t)); /* not */

	if (!rectangle) {
		perror("calloc() failed");
		exit(71); /* better, EX_OSERR */
	}

	/* Initialize the Rectangle members */
	//rectangle->height = 42; /* any random value */
	//rectangle->width = 23; /* any random value */
	


	/* Encode the Rectangle type as BER (DER) */
	ec = der_encode(&asn_DEF_Rectangle,
		rectangle, decode_callback, tab);

	if (ec.encoded == -1) {
		fprintf(stderr,
			"Could not encode Rectangle (at %s)\n",
			ec.failed_type ? ec.failed_type->name : "unknown");
		exit(65); /* better, EX_DATAERR */
	}
	else {
		fprintf(stderr, "Created %s with BER encoded Rectangle\n",
			"");
	}

	/* Also print the constructed Rectangle XER encoded (XML) */
	xer_fprint(stdout, &asn_DEF_Rectangle, rectangle);
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
