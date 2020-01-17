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
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define MAX_TEXT 512 //Declare Max size for data
#define MAX_ARR_SIZE 4 //Declare Record list size

struct msg_buf { // Structure for type of msg and data
	long msg_type;
	char some_text[MAX_TEXT];
} msgData;

struct Record { //Structure of the actaul data
	int index;
	char name[16];
	char departmentName[16];
	int employeeNum;
	int salary;
};
int Record_Num = 0; //Index of Records

//Function Prototypes
void add_item(struct Record *p, struct Record temp, int *Record_Num);

int main() {
	//Variable Declarations
	char str[MAX_TEXT];
	int running = 0;
	int msgid, msgid2;
	key_t recKey, sendKey;
	int commaCount = 0;
	char tempNum[12];
	char tempName[12];
	char tempSalary[12];
	int ret;
	struct msg_buf msgData, sendData;
	long int msg_to_receive = 1;
	int run = 1;
	recKey = ftok("/temp", 32); //Unique Key for recieving
	msgid = msgget(recKey, 0666 | IPC_CREAT);

	sendKey = ftok("/temp", 64); //Unique Key for sending
	msgid2 = msgget(sendKey, 0666 | IPC_CREAT);

	// data Array
	struct Record records[MAX_ARR_SIZE];
	{
		//Insertion of Test Data
		records[0].employeeNum = 1;
		strcpy(records[0].name, "TEST1");
		strcpy(records[0].departmentName, "Carleton");
		records[0].salary = 100;
		records[0].index = 0;
		Record_Num++;

		records[1].employeeNum = 2;
		strcpy(records[1].name, "Person 2");
		strcpy(records[1].departmentName, "Dep2");
		records[1].salary = 1000;
		records[1].index = 1;
		Record_Num++;

		records[2].employeeNum = 3;
		strcpy(records[2].name, "Person 3");
		strcpy(records[2].departmentName, "Dep3");
		records[2].salary = 2000;
		records[2].index = 2;
		Record_Num++;
	}

	struct Record temp;
	int choice;
	do {

		switch (choice) {
		//msgData.msg_type = 0;

		case 1: //Insert

			printf("Case 1: Insert\n");
			// add to Record
			if (Record_Num < MAX_ARR_SIZE) {//Do not go over bounds of record capacity
				temp.index = Record_Num; //Index corresponds to Record
				records[Record_Num] = temp; //Temporarily store current record
				Record_Num++; //Increase Index
			}
			printf("size of records %d \n", Record_Num);

			for (int i = 0; i < Record_Num; i++) {//Populate fields to send
				sprintf(sendData.some_text,"Add on Record # :%d ,Name:%s, Dep:%s, E-NUM :%d, Salary: %d",records[i].index, records[i].name,
						records[i].departmentName, records[i].employeeNum,records[i].salary);
				sendData.msg_type = 1;
				msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 1); //Send Message
			}
			choice = 0; //Start choices again
			break;

		case 2:
			//CheckName
			printf("Case 2: CheckName\n");
			int j = 0, k = 0, l = 0, m = 0, o=0;
			for (int i = 0; i < sizeof(msgData.some_text); i++) { //Iterate through all data
				if ((strncmp(&msgData.some_text[i], "/", 1)) == 0) { //Stop at the end
					break;
				}
				if ((strncmp(&msgData.some_text[i], ",", 1)) == 0) {//Find the indexes with commas
					ret = 0;// Find the index just after a comma
					commaCount++; //Increment commaCount
				} else {
					ret = 1;
				}

				if (commaCount == 1 && ret == 1) { // If at this index then this is the Employee Number
					tempNum[j] = msgData.some_text[i];
					j++;
				}
			}

			temp.employeeNum = atoi(tempNum); //Case String to int

			for (int i = 0; i < Record_Num; i++) { //Iterate through all records
				if(records[i].employeeNum == temp.employeeNum) //If the Record EmployeeNum matches input
					;
				sprintf(sendData.some_text,"Name: Name:%s", records[i].name); //Grab name corresponding to that employeee
				sendData.msg_type = 2;
				msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 0); //Send Data

			}
			choice = 0;
			break;
		case 3:
			//CheckDepatment
			printf("Case 3: CheckDepatment\n");
			j = 0, k = 0, l = 0, m = 0;
			for (int i = 0; i < sizeof(msgData.some_text); i++) { //Iterate through all data
				if ((strncmp(&msgData.some_text[i], "/", 1)) == 0) { //Stop at the end
					break;
				}
				if ((strncmp(&msgData.some_text[i], ",", 1)) == 0) { //Find the indexes with commas
					ret = 0;
					commaCount++;
				} else {
					ret = 1;
				}

				if (commaCount == 1 && ret == 1) { // If at this index then this is the Employee Number
					tempNum[j] = msgData.some_text[i];
					j++;
				}
			}

			temp.employeeNum = atoi(tempNum); //Case String to int

			for (int i = 0; i < Record_Num; i++) { //Iterate through all records
				if(records[i].employeeNum == temp.employeeNum){//If the Record EmployeeNum matches input
					sprintf(sendData.some_text,"Name: Name:%s", records[i].departmentName); //Grab Depatment name corresponding to that employeee
					sendData.msg_type = 2;
					msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 0); //Send Data
				}
			}
			choice = 0;

			break;
		case 4:
			//CheckSalary
			printf("Case 4: Check Salary\n");
			j = 0, k = 0, l = 0, m = 0;
			for (int i = 0; i < sizeof(msgData.some_text); i++) { //Iterate through all data
				if ((strncmp(&msgData.some_text[i], "/", 1)) == 0) { //Stop at the end
					break;
				}
				if ((strncmp(&msgData.some_text[i], ",", 1)) == 0) { //Find the indexes with commas
					ret = 0;
					commaCount++;
				} else {
					ret = 1;
				}

				if (commaCount == 1 && ret == 1) { // If at this index then this is the Employee Number
					tempNum[j] = msgData.some_text[i];
					j++;
				}
			}

			temp.employeeNum = atoi(tempNum); //Cast string to int

			for (int i = 0; i < Record_Num; i++) { //Iterate through all records
				if(records[i].employeeNum == temp.employeeNum){ //If the Record EmployeeNum matches input
					sprintf(sendData.some_text,"Name: Name:%d", records[i].salary);//Grab Depatment name corresponding to that employee
					sendData.msg_type = 2;
					msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 0);//Send Data
				}
			}
			choice = 0;

			break;
		case 5:
			//Check EmployeeNum

			printf("Case 5: Check Employee Number\n");
			j = 0, k = 0, l = 0, m = 0,o=0;
			for (int i = 0; i < sizeof(msgData.some_text); i++) {//Iterate through all data
				if ((strncmp(&msgData.some_text[i], "/", 1)) == 0) {//Stop at the end
					break;
				}
				if ((strncmp(&msgData.some_text[i], ",", 1)) == 0) {//Find the indexes with commas
					ret = 0;
					commaCount++;
				} else {
					ret = 1;
				}
				if (commaCount == 2 && ret == 1) { // If at this index then this is the Employee Name
					tempName[j] = msgData.some_text[i];
					j++;
				}
			}
			for (int i = 0; i < Record_Num; i++) {//Iterate through all records
				if(records[i].name == &tempName[o]){//If the Record EmployeeNum matches input
					o++;
					sprintf(sendData.some_text,"Name: Name:%s", records[i].departmentName);//Grab Depatment name corresponding to that employee
					sendData.msg_type = 2;
					msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 0);//Send Data
				}
			}
			choice = 0;

			break;
		case 6:
			//Check
			printf("Case 6: Check Employee Number by Department \n");
			for (int i = 0; i < Record_Num + 1; i++) {//Iterate through all records
				if (records[i].departmentName == temp.departmentName) {//If the Record EmployeeNum matches input
					records[i].employeeNum = *tempNum;//WHAT DOES THIS DO???
					sprintf(sendData.some_text,"Same Department at Record# :%d, Dep:%s, E-NUM :%d.",
							records[i].index, records[i].departmentName,records[i].employeeNum);

					msgsnd(msgid2, &sendData, sizeof(sendData.some_text), 0);//Send Data
					printf(" sendData Check: %s \n", sendData.some_text);
				}
			}			choice = 0;

			break;
		case 7:
			//Delete
			printf("Case 7: Delete Employee\n");
			for (int i = 0; i < Record_Num+1 ; i++) {//Iterate through all records
				if (records[i].employeeNum==temp.employeeNum) {//If the Record EmployeeNum matches input
					int last_index = Record_Num - 1; //Grab last index
					for (int i = records[i].index; i < last_index; i++) { //Find index to remove
						records[i] = records[i + 1];//Move indecies
					}
					Record_Num -= 1;//Remove
				}
			}
			choice = 0;
			break;
		case 9:
			run =0;
			break;
		default:
			printf("Ready to Recieve Data\n");

			msgrcv(msgid, &msgData, sizeof(msgData.some_text), 1, 0);//Wait for data

			//	printf(" RecData: %s \n", msgData.some_text);
			choice = atoi(&msgData.some_text[0]);//Convert String to int
			//	printf("choice %d\n", choice);

			{
				int j = 0, k = 0, l = 0, m = 0;
				for (int i = 0; i < sizeof(msgData.some_text); i++) {
					if ((strncmp(&msgData.some_text[i], "/", 1)) == 0) {
						break;
					}
					if ((strncmp(&msgData.some_text[i], ",", 1)) == 0) {
						ret = 0;
						commaCount++;
					} else {
						ret = 1;
					}

					if (commaCount == 1 && ret == 1) { // If at this index then this is the Employee Number
						tempNum[j] = msgData.some_text[i];
						j++;

					} else if (commaCount == 2 && ret == 1) {	// If at this index then this is the Employee Name
						temp.name[k] = msgData.some_text[i];
						k++;
					} else if (commaCount == 3 && ret == 1) {	// If at this index then this is the Department Name
						temp.departmentName[l] = msgData.some_text[i];
						l++;
					} else if (commaCount == 4 && ret == 1) {	// If at this index then this is the Salary
						tempSalary[m] = msgData.some_text[i];
						m++;
					}
				}
				temp.employeeNum = atoi(tempNum); //Cast str to int for E#
				temp.salary = atoi(tempSalary); //Case str to int for salary

				strcpy(msgData.some_text,"");
			}
			break;

		}

		sleep(2); //Wait for Admin to respond
	} while (run);
	msgctl(msgid, IPC_RMID, NULL);


	return 0;
}
void add_item(struct Record *p, struct Record temp, int *Record_Num) {
	if (*Record_Num < MAX_ARR_SIZE - 1) {
		p[*Record_Num] = temp;
		*Record_Num += 1;
	}
}
void delete_item(struct Record *p, int *Record_Num, int item) {
	if (*Record_Num > 0 && item < *Record_Num && item > -1) {
		int last_index = *Record_Num - 1;
		for (int i = item; i < last_index; i++) {
			p[i] = p[i + 1];
		}
		*Record_Num -= 1;
	}
}
