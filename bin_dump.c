#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <systype.h>
#include <sys/stat.h>
#include <time.h>


#define PATH_LEN 256
#define MD5_LEN 32

void HexDump_byte(FILE *fp, int size);
void HexDump_1010(FILE *fp, int size);
int Tsize_Check(FILE *fp);
//int file_type(int argc, char **argv);


void HexDump_byte(FILE *fp, int size)
{
        fseek(fp, 0, SEEK_SET);
        //fseek(fp, -2, SEEK_END);
        for(int i=0; i<size; i++){
        //for(int i=0; i<3; i++){
                printf("ftell(fp)=%08lx: ", ftell(fp));
                char c = fgetc(fp);
                printf(" c=%02X, feof=%d\n", c, feof(fp) );
        }
		fseek(fp, 0, SEEK_SET);
}


void HexDump_1010(FILE *fp, int size)
{
        while(1)
        {
            char asc[17];
            size_t i, j;
            asc[16]='\0';


            for( i=0; i< size ; i++)
            {

                    if(i%16 == 0)
                            printf("%08lX: ", ftell(fp));

                    char c = fgetc(fp);
                    printf("%02x", c);

                    //ascii special character
                    if( ( c>=' ' ) && ( c<='~') ){
                            asc[i%16] = c;
                    }
                    else {
                            asc[i%16] = '.';
                    }

                    if((i+1)%2 == 0) printf(" ");

                    if( (i+1) % 8 == 0 || (i+1) == size ){
                            printf(" ");

                            if( (i+1)%16 == 0 ){
                                    printf("| %s \n\r", asc);
                            }
                            else if( i+1 == size ){
                                    asc[(i+1)%16] = '\0';
                                    if( (i+1)%16 <= 8 )
                                            printf(" ");

                                    for( j= (i+1)%16 ; j<16; ++j){
                                            if(j%2==0)
                                                    printf(" ");
                                            if(j%8==0)
                                                    printf(" ");
                                            printf("  ");
                                    }
                                    printf("| %s \n\r", asc);
                                    break;
                            }
                            if(i+1 == size) break;
                    }
                    if( i+1 == size ) break;
            }
            if( i+1 == size ) break;
        }
}


int Tsize_Check(FILE *fp)
{
		int total_size=0;
		fseek(fp, 0, SEEK_END);
		total_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		return total_size;
}


int main(int argc, char* argv[])
{
		struct stat statbuf;

		if( argc != 2 )
		{
				printf("Incorrect command usage!\n");
				if( argc > 2){
						printf("Enter only 1 File Path.\n");
				}

				printf("Usage:Command [File Path] \n");
	            return 1;
    	}

    	int state;//for error detect in fclose()

		FILE *fp = fopen(argv[1], "rb"); //rb: 

		if(fp==NULL){
			printf("warning: Can't open file [%s]!\n", argv[1]);
			return 1;
		}

        printf("----------------------------------------------------------------------\n");
		printf("Address :  0 1  2 3  4 5  6 7   8 9  A B  C D  E F  | ascii value     \n");
		printf("----------------------------------------------------------------------\n");


		//file size check
		int size, total_size;

		//fseek(fp, 0, SEEK_END);
		total_size=Tsize_Check(fp);
		size = total_size;

		//fseek(fp, 0, SEEK_SET);

		if( total_size <= 0xa0)
		//HexDump_byte(fp, size);
				HexDump_1010(fp, size);

		else{
				HexDump_1010(fp, 0xa0);
				
				for(int i=0; i<7; i++){
						printf(".\n");
				}
				
				fseek(fp, -(0xa0+(total_size%16)), SEEK_END);
				//fseek(fp, -0xa0, SEEK_);
				HexDump_1010(fp, (0xa0+(total_size%16)));
		}

/*
 * ---------------------- result -------------------------
 * file-name       :
 * file type       :
 * file size(byte) :
 * chechsum(md5)   :
 * create_date     :
 *
 */

	  	printf("---------------------- result -------------------------\n");
		printf("file-name       : %s\n", argv[1]);

		printf("file type       :");	
	    FILE *fp2 = NULL;
    	char line[10240];
		char comm[1024] = "file u-boot.bin";
    	fp2 = popen(comm ,"r");
    	while(fgets(line, 10240, fp2) != NULL){
        	printf(" %s", line);
    	}

		printf("file size(byte) : %d byte, (0x%08X)\n", total_size, total_size);
		
		printf("chechsum(md5)   :");//%s\n, cksum);
		FILE *fp3 = NULL;
		char line2[10240];
		char comm2[1024] = "md5sum -b u-boot.bin";
		fp3 = popen(comm2, "r");
		while(fgets(line2, 10240, fp3) != NULL){
			printf(" %s", line2);
		}

		time_t ct = time(NULL);
		struct tm tm = *localtime(&ct);
		printf("create_date     : %d-%02d-%02d %d:%d:%d\n", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		state = fclose(fp);
		if(state!=0){
			printf("Error occur when fclose()\n");        
			return 2;        
		}
		return 0;
}
