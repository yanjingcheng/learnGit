#include <stdio.h>
#include <sys/types.h>
#include <Rectangle.h> /* Rectangle ASN.1 type */
#include <Triangle.h>
#include <sysexits.h>
/*
* This is a custom function which writes the
* encoded output into some FILE stream.
*/
 //asn1c  -S /usr/local/share/asn1c -fcompound-names -fskeletons-copy -gen-PER -pdu=auto 36331-c00.asn
static int write_out(const void *buffer, size_t size, void *app_key) {
	FILE *out_fp = app_key;
	size_t wrote;
	wrote = fwrite(buffer, 1, size, out_fp);
	return (wrote == size) ? 0 : -1;
}

int main(int ac, char **av) {
	Rectangle_t *rectangle; /* Type to encode */
	Triangle_t * triangle;

	asn_enc_rval_t ec; /* Encoder return value */
	/* Allocate the Rectangle_t */
	rectangle = calloc(1, sizeof(Rectangle_t)); /* not malloc! memset() after malloc*/
	triangle = calloc(1, sizeof(Triangle_t));
	if(!rectangle || !triangle) {
		perror("calloc() failed");
		exit(71); /* better, EX_OSERR */
	}
	/* Initialize the Rectangle members */
	rectangle->height = 142; /* any random value */
	rectangle->width = 423; /* any random value */
    rectangle->message.x = 1;
    rectangle->message.y = 2;
    rectangle->message.z = 3;
    rectangle->message.u = 2;
//	triangle->x = 1;
//	triangle->y = 2;
//	triangle->z = 3;

	/* BER encode the data if filename is given */
	if(ac < 3) {
		fprintf(stderr, "Sorry,Please Specify filename for BER output\n");
	} else {
			
			const char *filename = av[2];
			const char flag = atoi(av[1]);
			fprintf(stdout,"current type:%d\n",flag);
			FILE *fp = fopen(filename, "wb"); /* for BER output */
			if(!fp) {
				fprintf(stderr,"open result file failed!!!!\n");
				perror(filename);
				exit(71); /* better, EX_OSERR */
			}
		/* Encode the Rectangle type as BER (DER) */
		if(flag == 0){
			fprintf(stderr, "encode Rectangle...\n");
			ec = der_encode(&asn_DEF_Rectangle,rectangle, write_out, fp);
            if (ec.encoded < 0) {
                fprintf(stderr,
                    "Cannot convert %s into DER\n");
                exit(EX_UNAVAILABLE);
            }
		}
		else if(flag == 1){
			fprintf(stderr, "encode Triangle...\n");
			ec = der_encode(&asn_DEF_Triangle,triangle, write_out, fp);
		}
		else{
			fprintf(stderr, "sorry,either Triangle or Rectangle...\n");
		}
		fclose(fp);

		if(ec.encoded == -1) {
			fprintf(stderr,"Could not encode Rectangle (at %s)\n",ec.failed_type ? ec.failed_type->name : "unknown");
			exit(65); /* better, EX_DATAERR */
		} else {
		fprintf(stderr, "Created %s with BER encoded Rectangle\n",
		filename);
		}
		fprintf(stderr, "Encode successful!!\n");
		xer_fprint(stdout, &asn_DEF_Rectangle, rectangle);
	}
	/* Also print the constructed Rectangle XER encoded (XML) */
	
	return 0; /* Encoding finished successfully */
}
