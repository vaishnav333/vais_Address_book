#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    
    // Sort contacts based on the choosen criteria
    int criteria;
    printf("Enter sort criteria\n1.Name\n2.Phone\n3.email\n");
    scanf("%d",&criteria);

    switch(criteria)        //sorting according to criteria
    {
        case 1:sort_name(addressBook);
        break;
        case 2:sort_phone(addressBook);
        break;
        case 3:sort_email(addressBook);
        break;
    }

    for(int i=0;i<addressBook->contactCount;i++)        //print current contents
    {
        disp_i(addressBook,i);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization (After files)
    //loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    /* Define the logic to create a Contacts */

    //name
    int flag=1;
    char str[30];
    while(flag)      
    {
        printf("Enter the name\n");
        
        scanf(" %[^\n]",str);
        flag=validate_name(str);
        if(flag==1)
        {
            printf("Invalid name!!\nLength of the string should be minimum 2\n");
        }
        else if(flag==2)
        {
            printf("Invalid name!!\nInvalid character found in the entered string\n");
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].name,str);


    //phone 
    flag=1;
    char phn[15];
    while(flag)     
    {
        printf("Enter the phone number\n");
        scanf(" %[^\n]",phn);
        flag=validate_phone(phn,addressBook);
        
        if(flag==1)
        {
            printf("Invalid phone number!!(10 digits are required)\n");
        }
        else if(flag==2)
        {
            printf("Invalid phone number!!(character other than digit found)\n");
        }
        else if(flag==3)
        {
            printf("Invalid phone number!!(First number not between 6 and 9)\n");
        }
        else if(flag==4)
        {
            printf("Duplicate conctact!!(number already exist)\n");
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].phone,phn); 
    
    
    //email
    flag=1;
    char email[30];
    while(flag)
    {
        printf("enter the email id\n");
        scanf(" %[^\n]",email);
        flag=validate_email(email);
        if(flag)
        {
            printf("Invalid email id!!");
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].email,email);
    addressBook->contactCount++;    //increase count of contact after getting valid details
}

void searchContact(AddressBook *addressBook) 

{
    /* Define the logic for search */
    char input[60];
    int criteria;
    int i;

    printf("Enter search criteria\n1.Name\n2.Phone\n3.email\n");
    scanf("%d",&criteria);

    if(criteria<1||criteria>3)
    printf("Enter valid criteria number\n");

    printf("Enter criteria detail of the contact to search \n");
    scanf(" %[^\n]",input);

    i=find_i(addressBook,input,criteria);
    disp_i(addressBook,i);
    
    
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    char input[50];
    int criteria;
    int i;

    printf("Enter delete criteria\n1.Name\n2.Phone\n3.email\n");
    scanf("%d",&criteria);

    if(criteria<1||criteria>3)
    printf("Enter valid criteria number\n");

    printf("Enter criteria detail of the contact to delete \n");
    scanf(" %[^\n]",input);

    i=find_i(addressBook,input,criteria);
    delete_index(i,addressBook);
}

//validate_name
int validate_name(char *name)
{
    if(strlen(name)<2)
    {
        return 1;
    }
    for(int i=0;name[i]!='\0';i++)
    {
        if(!(('a'<=name[i] && name[i]<='z') || ('A'<=name[i] && name[i]<='Z') || ('0'<=name[i] && name[i]<='9') || name[i]==' '))
        return 2;
    }
    return 0;
}

//validate_phone
int validate_phone(char phn[], AddressBook *addressBook)
{
    if(strlen(phn)!=10)
    {
        return 1;         //check 10 digits are present or not
    }
    
    for(int i=0;i<strlen(phn);i++)
    {
        if(phn[i]<'0'||phn[i]>'9')
        return 2;          //check whether all are digits 
    }

    if(('0'<=phn[0]&&phn[0]<'6'))
    {
        return 3;           //check 1st digit btw 6-9
    }

    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(!strcmp(phn,addressBook->contacts[i].phone))
        return 4;           //check for duplicate phone numeber
    }

    return 0;

}

//validate_email
int validate_email(char email[])
{
    int at_count=0;
    char *at_address;

    if(email[0]=='@')
    return 1;       //cheking whether first char is '@'

    for(int i=0;i<strlen(email);i++)
    {
        if(email[i]>='A'&&email[i]<='Z') 
        return 1;               //check for uppercase

        if(email[i]=='@')
        {
            at_count+=1;
            at_address=&email[i];      //counting '@' count and storing its address
        }
    }

    if(at_count!=1)
    return 1;           //checking '@' occurs only one time

    char *dot_address;
    dot_address=strstr(email,".com");

    if(dot_address==0)
    return 1;

    if(&email[strlen(email)-4]!=dot_address)
    return 1;           //check last 4 char '.com'

    if(at_address+1==dot_address)
    return 1;           // checking a character is present between '@' and '.com'

    return 0;
}

void sort_name(AddressBook *addressBook) //sort based on name
{
    Contact temp;

    for(int i=0;i<addressBook->contactCount-1;i++)
    {
        for(int j=0;j<addressBook->contactCount-i-1;j++)
        {
            if(strcmp(addressBook->contacts[j].name,addressBook->contacts[j+1].name)>0)
            {
                
                temp=addressBook->contacts[j];
                addressBook->contacts[j]=addressBook->contacts[j+1];
                addressBook->contacts[j+1]=temp;
                
            }
        }
    }
}

void sort_phone(AddressBook *addressBook)   //sort based on phone
{
    Contact temp;

    for(int i=0;i<addressBook->contactCount-1;i++)
    {
        for(int j=0;j<addressBook->contactCount-i-1;j++)
        {
            if(strcmp(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone)>0)
            {
                
                temp=addressBook->contacts[j];
                addressBook->contacts[j]=addressBook->contacts[j+1];
                addressBook->contacts[j+1]=temp;
                
            }
        }
    }
}

void sort_email(AddressBook *addressBook)   //sort based on email
{
    Contact temp;

    for(int i=0;i<addressBook->contactCount-1;i++)
    {
        for(int j=0;j<addressBook->contactCount-i-1;j++)
        {
            if(strcmp(addressBook->contacts[j].email,addressBook->contacts[j+1].email)>0)
            {
                
                temp=addressBook->contacts[j];
                addressBook->contacts[j]=addressBook->contacts[j+1];
                addressBook->contacts[j+1]=temp;
                
            }
        }
    }
}

void delete_index(int i,AddressBook *addressBook)
{
    for(int j=i;j<addressBook->contactCount-1;j++)
    {
        addressBook->contacts[j]=addressBook->contacts[j+1];
    }
    addressBook->contactCount--;
}

int find_i(AddressBook*addressBook,char input[],int criteria_s)
{
    switch(criteria_s)
    {
        case 1:
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strstr(addressBook->contacts[i].name,input)) //strcmp=1 for exact string to be found
                return i;
            }
        case 2:
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strstr(addressBook->contacts[i].phone,input))
                return i;
            }
        case 3:
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strstr(addressBook->contacts[i].email,input))
                return i;
            }
        default:
        printf("Contact not found!!\n");
    }

}

void disp_i(AddressBook*addressBook,int i)
{
    printf("%s\t%s\t%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
}
