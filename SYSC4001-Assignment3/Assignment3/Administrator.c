/* SYSC 4001 Assignment 3
 *
 * Muhammad Tarequzzaman 100954008
 * Caleb Gryfe		101009798
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MAX_TEXT 512 //Declare Max size for data

struct msg_buf { // Structure for type of msg and data
	long  msg_type;
	char some_text[MAX_TEXT];
} msgData;


struct Record { //Structure of the actaul data
	long type;
	struct Data {
		char name[12];
		char departmentName[12];
		int employeeNum;
		int salary;
	} data;
};
//Function Prototypes
void insert(struct msg_buf msgData);
void sendData(int msgId, struct msg_buf msgData);


int main() {
	key_t sendKey,recKey; // Identifiers for sending / recieving
	int msgid, msgid2;
	struct msg_buf msgData, getdata;//Instances of the structure
	int run= 1;
	sendKey = ftok("/temp", 32);//Unique Key for sending
	msgid = msgget(sendKey, 0666|IPC_CREAT);

	recKey = ftok("/temp",64); //Unique Key for recieving
	msgid2 = msgget(recKey, 0666|IPC_CREAT);

	do {//Alwyas display the menu
		int inputType;
		printf("1. Insert\n");
		printf("2. Check Name\n");
		printf("3. Check Department\n");
		printf("4. Check Salary\n");
		printf("5. Check Employee Number\n");
		printf("6. Check\n");
		printf("7. Delete\n");
		printf("9. EXIT \n");

		printf("Please Enter a Number corresponding to one of the options above: ");

		scanf("%d", &inputType);//Input user choice
		msgData.msg_type = 1;

		switch (inputType) {
			case 1: //Insert Functionality on Admin Side
				printf("Case 1: Insert\n");
				char name[16];
				char departmentName[16];
				int employeeNum;
				int salary;
				char str[BUFSIZ];
					//Poll User for input data
					printf("Enter the Employee Name: ");
					scanf("%s", name);
					printf("Enter the Department Name: ");
					scanf("%s", departmentName);
					printf("Enter the Employee Number: ");
					scanf("%d", &employeeNum);
					printf("Enter the Salary: ");
					scanf("%d", &salary);

					//Copy data into a string in order to send
					sprintf(str, "%d,%d,%s,%s,%d/", 1, employeeNum, name, departmentName,salary);
					strcpy(msgData.some_text, str);

				//Send Message data in predetermined format
				msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
				printf(" send data: %s \n", msgData.some_text);

				// Recieve data from the Record Keeper
				msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 1, 0);
				printf("get : %s \n", getdata.some_text);
				//msgctl(msgid2, IPC_RMID, NULL);
				break;

				case 2://CheckName Functionality on Admin Side

				 printf("Case 2: Check Name\n");
				 //Receive Employee Number
				 printf("Enter the Employee Number: ");
				 scanf("%d", &employeeNum);
				 //Copy data into a string in order to send
				 sprintf(str, "%d,%d,%s,%s,%d/", 2, employeeNum, "0", "0",0);
				 strcpy(msgData.some_text, str);

				 //Send Message data in predetermined format
				 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
				 printf(" send data: %s \n", msgData.some_text);

				 // Recieve data from the Record Keeper
				 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
				 printf("get : %s \n", getdata.some_text);

				// msgctl(msgid2, IPC_RMID, NULL);

				 break;

				 case 3://CheckDepatment
					 printf("Case 3: Check Depatment\n");
					 printf("Enter the Employee Number: ");
					 scanf("%d", &employeeNum);
					 //Copy data into a string in order to send
					 sprintf(str, "%d,%d,%s,%s,%d/", 3, employeeNum, "0", "0",0);
					 strcpy(msgData.some_text, str);

					 //Send Message data in predetermined format
					 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
					 printf(" send data: %s \n", msgData.some_text);

					 // Recieve data from the Record Keeper
					 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
					 printf("get : %s \n", getdata.some_text);

					// msgctl(msgid2, IPC_RMID, NULL);
					 break;

				 case 4://CheckSalary
					 printf("Case 4: Check Salary\n");
					 printf("Enter the Employee Number: ");
					 scanf("%d", &employeeNum);
					 //Copy data into a string in order to send
					 sprintf(str, "%d,%d,%s,%s,%d/", 4, employeeNum, "0", "0",0);
					 strcpy(msgData.some_text, str);

					 //Send Message data in predetermined format
					 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
					 printf(" send data: %s \n", msgData.some_text);

					 // Recieve data from the Record Keeper
					 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
					 printf("get : %s \n", getdata.some_text);

					// msgctl(msgid2, IPC_RMID, NULL);
					 break;

				 case 5://CheckEmployeeNum
				 printf("Case 5: Check Employee Number\n");
				 printf("Enter the Employee Name: ");
				 scanf("%s", name);
				 sprintf(str, "%d,%d,%s,%s,%d/", 5, 0, name, "0",0);
				 strcpy(msgData.some_text, str);

				 //Send Message data in predetermined format
				 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
				 printf(" send data: %s \n", msgData.some_text);

				 // Recieve data from the Record Keeper
				 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
				 printf("get : %s \n", getdata.some_text);

				// msgctl(msgid2, IPC_RMID, NULL);
				 break;

				 case 6://Check
					 printf("Case 6: Check\n");
					 printf("Enter the Department Name: ");
					 scanf("%s", departmentName);
					 //Copy data into a string in order to send
					 sprintf(str, "%d,%d,%s,%s,%d/", 6, 0, "0", departmentName,0);
					 strcpy(msgData.some_text, str);

					 //Send Message data in predetermined format
					 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
					 printf(" send data: %s \n", msgData.some_text);

					 // Recieve data from the Record Keeper
					 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
					 printf("get : %s \n", getdata.some_text);

					// msgctl(msgid2, IPC_RMID, NULL);
					 break;

				 case 7://Delete
					 printf("Case 7: Delete Employee\n");
					 printf("Enter the Employee Number: ");
					 scanf("%d", &employeeNum);
					 //Copy data into a string in order to send
					 sprintf(str, "%d,%d,%s,%s,%d/", 7, employeeNum, "0", "0",0);
					 strcpy(msgData.some_text, str);

					 //Send Message data in predetermined format
					 msgsnd(msgid, &msgData, sizeof(msgData.some_text), 0);
					 printf(" send data: %s \n", msgData.some_text);

					 // Recieve data from the Record Keeper
					 msgrcv(msgid2,  &getdata, sizeof(getdata.some_text), 2, 0);
					 printf("get : %s \n", getdata.some_text);

					// msgctl(msgid2, IPC_RMID, NULL);
					 break;
				 case 9:
					 run = 0;
					 break;
				 default:
				 printf("Invalid Entry, Please pick from the list\n");
				 break;
		}
	} while (run);

	 msgctl(msgid2, IPC_RMID, NULL);

	return 0;
}

void sendData(int msgId, struct msg_buf msgData) {
	//int len= strlen(msgData.some_text) + 1;
	msgsnd(msgId, &msgData, (sizeof(msgData)), 0);

}
