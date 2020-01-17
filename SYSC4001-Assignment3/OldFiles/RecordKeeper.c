
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>




struct my_msg_st {
	long int my_msg_type;
	char some_text[BUFSIZ];
}test;
struct oneRecord{
  char name[12];
  char departmentName[12];
  int employeeNum;
  int salary;
}record;

void insert(struct my_msg_st);

int main() {

	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;


	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running) {
		if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
			msg_to_receive, 0) == -1) {
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
    //insert(some_data);
    //printf("Get here?\n");
		//for(int i=0;i<12;i++){
		//	printf("Test: %c",some_data.some_text[i]);
		//}
		printf("You wrote: %s\n", some_data.some_text);
		exit(EXIT_SUCCESS);
    if (strncmp(some_data.some_text, "end", 2) == 0) {
			 running = 0;
		 }
	 }
	if (msgctl(msgid, IPC_RMID, 2) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n"); exit(EXIT_FAILURE);
}
	exit(EXIT_SUCCESS);
}

void insert(struct my_msg_st some_data){
	FILE *fp;
	FILE *fp2;

  int commaCount = 0;
  char tempNum[12];
  char tempSalary[12];
	int ret;

	fp = fopen("RecordFile.txt","w");
	printf("You wrote: %s\n", some_data.some_text);
	if(feof(fp)){
		fputs(some_data.some_text,fp);
		fclose(fp);
	}else{
		printf("Did is get here?\n");
		fclose(fp);
		fp2 = fopen("RecordFile.txt","a");
		fputs(some_data.some_text,fp2);
		fclose(fp2);
	}





//  for(int i=0;i<48;i++){
// 		ret = strncmp(&some_data.some_text[i], ",",1);
//
// 		if(ret==0){
// 			commaCount++;
// 			printf("There is a comma: %c\n", some_data.some_text[i]);
// 		}
//     if(commaCount == 0){
//       //printf("Test 1\n");
//       record.name[i] = some_data.some_text[i];
//       //printf("Name 1: %s\n",record.name);
//
//     }else if(commaCount == 1){
//       //printf("Test 2\n");
//       record.departmentName[i] = some_data.some_text[i];
//       printf("D-Name 1: %s\n",record.departmentName);
//
//     }else if(commaCount == 2){
//       tempNum[i] = some_data.some_text[i];
// 			printf("E-Num 1: %s\n",record.departmentName);
//     }else{
//       tempSalary[i] = some_data.some_text[i];
// 			printf("Salary 1: %s\n",record.departmentName);
//     }
//
//   }
//   record.employeeNum = atoi(tempNum);
//   record.salary = atoi(tempSalary);
//
// 	printf("Size of some_data: %lu\n",sizeof(some_data));
//   printf("Name: %s, D-Name: %s, E-Num: %d, Salary: %d\n",record.name,record.departmentName,record.employeeNum,record.salary);
}
