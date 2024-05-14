#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<fcntl.h>

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock 
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK , *PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int permission;
	struct inode *next;
}INODE , *PINODE , **PPINODE;

typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE,*PFILETABLE;

typedef struct ufdt 
{
	PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;


void InitialiseSuperBlock()
{
	int i = 0;
	while(i < MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;
}

void CreateDILB()
{
	int i = 1;
	PINODE newn = NULL;
	PINODE temp = head;
	
	while(i <= MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));
		
		newn->LinkCount = 0;
		newn->ReferenceCount = 0;
		newn->FileType = 0;
		newn->FileSize = 0;
		
			newn->Buffer = NULL;
			newn->next = NULL;
		
		newn->InodeNumber = i;
		
		if(temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}
	printf("\tDILB created Succesfullly\n");
}

PINODE Get_Inode(char *name)
{
	PINODE temp = head;
	int i = 0;
	
	if(name == NULL)
	{
		return NULL;
	}
	
	while(temp != NULL)
	{
		if(strcmp(name , temp->FileName ) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	
	return temp;
}
int CreateFile(char *name , int permission)
{
	int i = 0;
	PINODE temp = head;
	
	if((name == NULL) || (permission == 0) || (permission > 3))
	{
		return -1;
	}
	
	//their is not inodes are remain
	if(SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}
	
	//file is already present 
	if(Get_Inode(name) != NULL)
	{
		return -3;
	}
	
	(SUPERBLOCKobj.FreeInode)--;
	
	while(temp != NULL)
	{
		if(temp->FileType == 0)
		{
			break;
		}
		temp = temp->next;
	}
	
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	
	//setting the value of filetable
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = permission;
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	
	//setting the value of IIT
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	
	strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName , name);
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);
	
	return i;
}

void LS_File()
{
	int i = 0;
	PINODE temp = head;
	
	if(SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("\n\t ERROR : There is no such file");
		return ;
	}
	
	printf("\n\t File Name \tINODE Number \t FileSize \t Link count\n");
	printf("\n\t----------------------------------------------------------\n");
	while(temp != NULL)
	{
		if(temp->FileType != 0)
		{
			printf("\n\t %s \t %d \t\t %d \t\t %d\n",temp->FileName , temp->InodeNumber , temp->FileActualSize,temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("\n\t----------------------------------------------------------\n");
}

int GetFDFromName(char *name)
{
	int i = 0;
	
	while(i < 50)
	{
		//go inside when pointer to file table is not equal to null
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName ), name) == 0)
			{
				break;
			}
		}
		i++;
	}
	
	if(i == 50)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

int WriteFile(int fd , char *arr , int isize )
{
	if(((UFDTArr[fd].ptrfiletable->mode) != WRITE ) && ((UFDTArr[fd].ptrfiletable->mode) != WRITE + READ))
	{
		return -1;
	}
	
	if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE ) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE + READ ))
	{
		return -1;
	}
	
	if((UFDTArr[fd].ptrfiletable->writeoffset ) == MAXFILESIZE)
	{
		return -2;
	}
	
	if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR )
	{
		return -3;
	}
	
	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);
	
	(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;
	
	(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;	
	
	return isize;
}

int ReadFile(int fd, char *arr , int isize)
{
	int read_size = 0;
	
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}
	
	if(UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ + WRITE )
	{
		return -2;
	}
	
	if(UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
	{
		return -3;
	}
	
	if(UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
	{
		return -4;
	}
	
	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
	
	if(read_size < isize)
	{
		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),read_size);

		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
	}
	else{
		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);

		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + isize;
	}	
	return isize;
	
}

int Stat_File(char *name )
{
	PINODE temp = head;
	int i = 0;
	
	if(name == NULL)
	{
		return -1;
	}
	
	while(temp != NULL)
	{
		if(strcmp(name , temp->FileName) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	
	if(temp == NULL)
	{
		return -2;
	}
	
	printf("\n\t -----------------------STATISTICAL INFORMATION ABOUT FILE-----------------------");
	printf("\n\t File Name :%s",temp->FileName);
	printf("\n\t Inode Number :%d",temp->InodeNumber);
	printf("\n\t File Size :%d",temp->FileSize);
	printf("\n\t File Actula size :%d",temp->FileActualSize);		
	printf("\n\t Link Count: %d",temp->LinkCount);
	printf("\n\t Reference count :%d",temp->ReferenceCount);

	if(temp->permission == 1)
	{
		printf("\n\t File permission : Read only");
	}else if(temp->permission == 2)
	{
		printf("\n\t File permission : Write only");
	}else if(temp->permission == 3)
	{
		printf("\n\t File permission : Read and Write ");
	}
	return 0;
}

int FStat_File(int fd)
{
	PINODE temp = head;
	int i = 0;
	
	if(fd < 0)
	{
		return -1;
	}
	
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -2;
	}
	
	//assigning the temp value as inode value
	temp = UFDTArr[fd].ptrfiletable->ptrinode;
	
	printf("\n\t -----------------------STATISTICAL INFORMATION ABOUT FILE-----------------------");
	printf("\n\t File Name :%s",temp->FileName);
	printf("\n\t Inode Number :%d",temp->InodeNumber);
	printf("\n\t File Size :%d",temp->FileSize);
	printf("\n\t File Actula size :%d",temp->FileActualSize);		
	printf("\n\t Link Count: %d",temp->LinkCount);
	printf("\n\t Reference count :%d",temp->ReferenceCount);

	if(temp->permission == 1)
	{
		printf("\n\t File permission : Read only");
	}else if(temp->permission == 2)
	{
		printf("\n\t File permission : Write only");
	}else if(temp->permission == 3)
	{
		printf("\n\t File permission : Read and Write ");
	}
	return 0;
}

int RM_File(char *name)
{
	int fd = 0;
	fd = GetFDFromName(name);
	if(fd == -1)
	{
		return -1;
	}
	
	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;
	
	if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);
	}
	
	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;
	
	printf("\n\t File deleted Succesfully");
	return 0;
}
void DisplayHelp()
{
	printf("\n\t -------------------------------------------------------------------------------------------");
	printf("\n\t ls : To list out all files");
	printf("\n\t cls : To clear Console ");
	printf("\n\t open : To oen the file");
	printf("\n\t close : To close");
	printf("\n\t closeall : To close all opened files");
	printf("\n\t read : To Read the contents into file ");
	printf("\n\t write : To write contents into file");
	printf("\n\t exit : To Terminate the file system");
	printf("\n\t stat : To Display informaction of file using name");	
	printf("\n\t fstat : To Display informaction of file using file descriptor");
	printf("\n\t truncate : To Remove all data from file");
	printf("\n\t rm : To Delet the file");
	printf("\n\t -------------------------------------------------------------------------------------------");
}

void man(char *name)
{
	if(name == NULL)
	{
		return ;
	}
	
	printf("\n\t -------------------------------------------------------------------------------------------");
	if(strcmp(name , "create")==0)
	{
		printf("\n\t Description : Used to crreate new regular file");
		printf("\n\tUsage : create File_name Permission");
	}else if(strcmp(name , "read")==0)
	{
		printf("\n\t Description : Used to read data from regular file ");
		printf("\n\t Usage : read File_name No_Of_Bytes_To_Read");
	}else if(strcmp(name , "write")==0)
	{
		printf("\n\t Description : Used to write data in regular file ");
		printf("\n\t Usage : write File_name \n After this enter the data that we want to write");
	}else if(strcmp(name , "ls")==0)
	{
		printf("\n\t Description : Used to list all information of file ");
		printf("\n\t Usage : ls ");
	}else if(strcmp(name , "stat")==0)
	{
		printf("\n\t Description : Used to Display all the informaction about file");
		printf("\n\t Usage : ls ");
	}else if(strcmp(name , "fstat")==0)
	{
		printf("\n\t Description : Used to display all information of file ");
		printf("\n\t Usage : ls ");
	}else if(strcmp(name , "truncate")==0)
	{
		printf("\n\t Description : used to remove data from file ");
		printf("\n\t Usage : truncate File_name ");
	}else if(strcmp(name , "open")==0)
	{
		printf("\n\t Description : Used to open file ");
		printf("\n\t Usage : open File_Name mode ");
	}else if(strcmp(name , "close")==0)
	{
		printf("\n\t Description : Used to close opened file ");
		printf("\n\t Usage : close File_Name");
	}else if(strcmp(name , "closeall")==0)
	{
		printf("\n\t Description : Used to close all opened file ");
		printf("\n\t Usage : closeall");
	}else if(strcmp(name , "lseek")==0)
	{
		printf("\n\t Description : Used to change the fileoffset ");
		printf("\n\t Usage : lseek File_Name changeInOffset startPoint");
	}
	else if(strcmp(name , "rm")==0)
	{
		printf("\n\t Description : Used to delete the file");
		printf("\n\t Usage : rm File_Name");
	}else {
		printf("\n\t ERROR : No manual entry available .");
	}
	printf("\n\t -------------------------------------------------------------------------------------------");
}

void Save_All_Files()
{
	PINODE temp = head;
	int ret = 0 , fd = 0;
	
	char arr[2048];
	strcpy(arr,"");
	
	while(temp != NULL)
	{
		if(temp->FileType != 0)
		{
			ret = creat(temp->FileName , 0777);
			if(ret == -1)
			{
				printf("\n\t Unable to create file");
			}
			
			fd = open(temp->FileName , O_RDWR);
			if(fd == -1)
			{
				printf("\n\t Unable to Open file");
			}
					
			write(fd , temp->Buffer , temp->FileActualSize);
		}
		
		temp = temp->next;
	}
	
	printf("\n\t File Saved succesfully");
}

int OpenFile(char *name ,int mode )
{
	int i = 0;
	PINODE temp = NULL;
	
	if(name == NULL || mode <= 0)
	{
		return -1;
	}
	
	temp = Get_Inode(name);
	
	if(temp == NULL)
	{
		return -2;
	}
	
	if(temp->permission < mode)
	{
		return -3;
	}
	
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if(UFDTArr[i].ptrfiletable == NULL)
	{
		return -1;
	}
	
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = mode;
	
	if(mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}else if(mode == READ)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
	}else if(mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	
	return i;
}


int main()
{
	char *ptr = NULL;
	int ret = 0 , fd = 0, count = 0;
	char command[4][80] , str[80] , arr[1024];
	
	InitialiseSuperBlock();
	CreateDILB();
	
	
	while(1)
	{
		fflush(stdin);
		strcpy(str,"");
		
		printf("\n\t i's VFS : >>");
		
		fgets(str , 80 , stdin);
		
		count = sscanf(str , "%s %s %s %s",command[0],command[1],command[2],command[3]);
		
		if(count == 1)
		{
			if(strcmp(command[0] ,"exit") == 0)
			{
				Save_All_Files();
				printf("\tTerminating the i's Virtula File System\n");
				break;
			}else if(strcmp(command[0] ,"ls") == 0)
			{
				LS_File();
			}else if(strcmp(command[0] ,"cls") == 0)
			{
				system("clear");
				continue;
			}else if(strcmp(command[0] ,"help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else
			{
				printf("\n\tERROR : Invalid Input ");
			}
					
		}else if(count == 2)
		{
			if(strcmp(command[0] , "write")==0)
			{
				fd = GetFDFromName(command[1]);
				if(fd == -1)
				{
					printf("\n\t ERROR : Incorrect Parameter\n");
				}
				
				printf("\n\t Enter Data :");
				scanf("%[^\n]",arr);
				
				ret = strlen(arr);
				
				if(ret == 0)
				{
					printf("\n\t ERROR : Incorrect Parameter \n");
					continue;
				}
				ret = WriteFile(fd , arr , ret );
				if(ret == -1)
				{
					printf("\n\t ERROR : Permission Denied");
				}
				else if(ret == -2)
				{
					printf("\n\t ERROR : There is no sufficient memory to write ");
				}else if(ret == -3)
				{
					printf("\n\t ERROR : It is not regular file");
				}
			}else if(strcmp(command[0] , "stat") == 0)
			{
				ret = Stat_File(command[1]);
				if(ret == -1)
				{
					printf("\n\t ERROR : Incorrect Parameter");
				}else if(ret == -2)
				{
					printf("\n\t ERROR : There is no such file");
					continue;
				}
				
			}else if(strcmp(command[0] , "fstat") == 0)
			{
				ret = FStat_File(atoi(command[1]));
				if(ret == -1)
				{
					printf("\n\t ERROR : Incorrect Parameter");
				}else if(ret == -2)
				{
					printf("\n\t ERROR : There is no such file");
					continue;
				}
				
			}else if(strcmp(command[0],"rm") == 0)
			{
				ret = RM_File(command[1]);
				if(ret == -1)
				{
					printf("\n \t ERROR : There is no such file");
					continue;
				}
			}else if(strcmp(command[0],"man") == 0)
			{
				man(command[1]);
			}
		}
		else if(count == 3)
		{
			if(strcmp(command[0] , "create") == 0)
			{
				ret = CreateFile(command[1] , atoi(command[2]));
				if(ret >= 0)
				{
					printf("\n\t File is Succesfully created with file descriptor :%d",ret);
				}
				else if(ret == -1)
				{
					printf("\n\tERROR : Incorrect parameter ");
				}else if(ret == -2)
				{
					printf("\n\t ERROR : There is no inodes ");
				}else if(ret == -3)
				{
					printf("\n\t ERROR : File is already exists ");
				}
				else if(ret == -4)
				{
					printf("\n\t ERROR : Memory allocation failure ");
				}
				else
				{
					printf("\n\tERROR : Invalid Input ");
				}
				continue;
			}
			else if(strcmp(command[0] , "read") == 0)
			{
				fd = GetFDFromName(command[1]);
				
				if(fd == -1)
				{
					printf("\n\t ERROR : Incorrect Parameter");
					continue;
				}
				
				ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);
				
				if(ptr == NULL)
				{
					printf("\n\t Memory allocation Failur");
					continue;
				}
				
				ret = ReadFile(fd , ptr , atoi(command[2]));
				
				if(ret == -1)
				{
					printf("\n\t ERROR : No such file");
				}else if(ret == -2)
				{
					printf("\n\t ERROR : Permission Denied");
				}else if(ret == -3)
				{
					printf("\n\t ERROR : Reached at end of file");
				}else if(ret == -4)
				{
					printf("\n\t ERROR : File is not regular");
				}else if(ret == 0)
				{
					printf("\n\t ERROR : File is empty");
				}
				if(ret > 0)					
				{	
				        printf("\n\tData :");
					printf("\n\t---------------------------------------------------------------------------\n");
					write(2 , ptr , ret);
					printf("\n\t---------------------------------------------------------------------------");
				}
				continue;
			}else if(strcmp(command[0] , "open") == 0)
			{
				ret = OpenFile(command[1],atoi(command[2]));
				if(ret >= 0)
				{
					printf("\n\t File is succesfully opened with file descriptor %d",ret);
				}
				if(ret == -1 )
				{
					printf("\n\t ERROR : Incorrect Parameter ");
				}
				
				if(ret == -2 )
				{
					printf("\n\t ERROR : File not present ");
				}
				if(ret == -3 )
				{
					printf("\n\t ERROR : Permission Denied ");
				}
				continue;
			}
			
		
		}
		
	}
	
	return 0;
}

/*
	ls 
	clear 
	help 
	exit
	stat 
	fstat 
	rm
	write
	create
	read
	man
	open
	
	closeall
	close
	truncate
	lseek

*/






















