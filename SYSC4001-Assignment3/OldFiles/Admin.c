#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>




#define MAX_TEXT 512

struct my_msg_st {
	long int my_msg_type;
	char some_text[MAX_TEXT];
};

void promptMenu();
void insert();
void checkName();
void checkDepatment();
void checkSalary();
void checkEmployeeNum();
void check();
void delete();
void sendToRecordKeeper(char name[], char departmentName[], int employeeNum, int salary);
void sendRecord(char sendString[]);


int main() {
		promptMenu();
}


void promptMenu(){
	int input;
	printf("1. Insert\n");
	printf("2. Check Name\n");
	printf("3. Check Department\n");
	printf("4. Check Salary\n");
	printf("5. Check Employee Number\n");
	printf("6. Check\n");
	printf("7. Delete\n");
	printf("Please Enter a Number corresponding to one of the options above: ");
	scanf("%d",&input);

	switch(input){
		case 1:
			printf("Case 1\n");
			insert();
			break;
		case 2:
			printf("Case 2\n");
			checkName();
			break;
		case 3:
			printf("Case 3\n");
			checkDepatment();
			break;
		case 4:
			printf("Case 4\n");
			checkSalary();
			break;
		case 5:
			printf("Case 5\n");
			checkEmployeeNum();
			break;
		case 6:
			printf("Case 6\n");
			check();
			break;
		case 7:
			printf("Case 7\n");
			delete();
			break;

		default:
			printf("Invalid Entry, Please pick from the list\n");
			break;
		}

}

void insert(/*char name[], char departmentName[], int employeeNum, int salary*/){
	char name[12];
	char departmentName[12];
	int employeeNum;
	int salary;

	printf("Enter the Employee Name: ");
	scanf("%s",name);

	printf("Enter the Department Name: ");
	scanf("%s",departmentName);

	printf("Enter the Employee Number: ");
	scanf("%d",&employeeNum);

	printf("Enter the Salary: ");
	scanf("%d",&salary);

	sendToRecordKeeper(name,departmentName,employeeNum,salary);

}
void checkName(/*int employeeNum*/){
	int employeeNum;
	printf("Enter the Employee Number: ");
	scanf("%d",&employeeNum);
	sendToRecordKeeper("2","0",employeeNum,0);
}
void checkDepatment(/*int employeeNum*/){
	int employeeNum;

	printf("Enter the Employee Number: ");
	scanf("%d",&employeeNum);

	sendToRecordKeeper("3","0",employeeNum,0);
}
void checkSalary(/*int employeeNum*/){
	int employeeNum;

	printf("Enter the Employee Number: ");
	scanf("%d",&employeeNum);

	sendToRecordKeeper("4","0",employeeNum,0);
}
void checkEmployeeNum(/*char name[]*/){
	char name[12];

	printf("Enter the Employee Name: ");
	scanf("%s",name);

	sendToRecordKeeper(name,"0",0,0);
}
void check(/*char departmentName[]*/){
	char departmentName[12];

	printf("Enter the Department Name: ");
	scanf("%s",departmentName);

	sendToRecordKeeper("6",departmentName,0,0);
}
void delete(/*int employeeNum*/){
	int employeeNum;

	printf("Enter the Employee Number: ");
	scanf("%d",&employeeNum);

	sendToRecordKeeper("7","0",employeeNum,0);
}

void sendToRecordKeeper(char name[], char departmentName[], int employeeNum, int salary){
	char sendString[60];
	char test[12];
	char t2[10];
	if(strncmp(name, "2",12) == 0){
		printf("%s\n", name);
		printf("Hit 2\n");
		// 2.Check Name: Send Employee Number to record Keeper
		sendString[0] = '2';
		sendString[1] = ',';
		snprintf(test, 12, "%d",employeeNum);//msg, sizeof(msg), "%d", 1
		printf("Test contains: %s\n",test);
		sendString[2] = test;
		// for(int i=2;i<sizeof(name);i++){
		// 	sendString[i] = name[i-2];
		// 	printf("%c\n", sendString[i]);
		// }
		//sendRecord(sendString);

	}else if(strncmp(name,"3",12) == 0){
		printf("Hit 3\n");
		// 3.Check Department: Send Employee Number to record Keeper
		sendString[0] = '3';
		sendString[1] = ',';
		sendString[2] = employeeNum;
		printf("Employee Number: %d\n", employeeNum);

	}else if(strncmp(name, "4",12) == 0){
		printf("Hit 4\n");
		// 4.Check Salary: Send Employee Number to record Keeper
		sendString[0] = '4';
		sendString[1] = ',';
		sendString[2] = employeeNum;
	}else if(strncmp(name,"6",12) == 0){
		printf("Hit 6\n");
		// 6.Check: Send Department Name to record Keeper
		sendString[0] = '6';
		sendString[1] = ',';
		//printf("String length is: %lu\n", strlen(departmentName));
		for(int i=2;i<strlen(departmentName);i++){
		 	sendString[i] = departmentName[i-2];
			printf("%c\n", sendString[i]);
		 }
	}else if(strncmp(name, "7",12) == 0){
		printf("Hit 7\n");
		// 7.Delete: Send Employee Number to record Keeper
		sendString[0] = '7';
		sendString[1] = ',';
		sendString[2] = employeeNum;
	}else if(name != NULL && employeeNum == 0){
		printf("Hit 5 Test Hit here!\n");
		// 5.Check Employee Number: Send Name to record keeper
		sendString[0] = '5';
		sendString[1] = ',';
		for(int i=2;i<strlen(name);i++){
		 	sendString[i] = name[i-2];
			printf("%c\n", sendString[i]);
		 }
	}else{
		printf("Hit 1\n");
		sendString[0] = '1';
		sendString[1] = ',';
		// 1.Insert: Send all info to record keeper
		for(int i=2;i<(strlen(name)+strlen(departmentName)+24);i++){
			if(i<strlen(name)){
				sendString[i] = name[i-2];
			}else if(i>strlen(name) && i <(strlen(name)+strlen(departmentName))){
				sendString[i] = departmentName[i-2];
			}else if(i>(strlen(name)+strlen(departmentName) && i<(strlen(name)+strlen(departmentName)+12))){
				sendString[i] = employeeNum;
			}else if(i>(strlen(name)+strlen(departmentName)+12) && i<(strlen(name)+strlen(departmentName)+24)){
				sendString[i] = salary;
			}
			printf("%c\n", sendString[i]);
		 }
	}
	printf("In Send String: %s\n",sendString);
	sendRecord(sendString);
}

void sendRecord(char sendString[]){
	int running = 2;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	//do{
		//printf("Enter some text: ");
		//fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 2;
		strcpy(some_data.some_text, sendString);

		if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1){
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if (strncmp(buffer, "end", 2) == 0) {
			running = 0;
		}
		running = running-1;
	//}while(running);
	exit(EXIT_SUCCESS);

}
