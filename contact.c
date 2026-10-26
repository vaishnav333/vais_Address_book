#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    
    // Sort contacts based on the choosen criteria
    int criteria;
    if(addressBook->contactCount==0)
    {
        printf("No contact found!!");
        return ;
    }
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

    disp_heading();
    for(int i=0;i<addressBook->contactCount;i++)        //print current contents
    {
        disp_i(addressBook,i);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
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
        printf("Enter the name:\n");
        
        scanf(" %[^\n]",str);
        flag=validate_name(str,addressBook);    //validate name

        switch(flag)
        {   //error messages for name
            case 1:printf("Invalid name!!\n(Length of the string should be minimum 2)\n");
            break;
            case 2:printf("Invalid name!!\n(Invalid character found in the entered string)\n");
            break;
            case 3:printf("Duplicate contact exist\n");
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].name,str);  //copy


    //phone 
    flag=1;
    char phn[15];
    while(flag)     
    {
        printf("Enter the phone number\n");
        scanf(" %[^\n]",phn);
        flag=validate_phone(phn,addressBook);   //validate phone number
        
        switch(flag)
        {   //error messages for phone
            case 1:printf("Invalid phone number!!(10 digits are required)\n");
            break;
            case 2:printf("Invalid phone number!!(character other than digit found)\n");
            break;
            case 3:printf("Invalid phone number!!(First number not between 6 and 9)\n");
            break;
            case 4:printf("Duplicate conctact!!(number already exist)\n");
            break;
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
        flag=validate_email(email,addressBook); //validate email

        switch(flag)
        {   //error message for email
            case 1:printf("Invalid email id!!(first char should be alphabet or number)");
            break;
            case 2:printf("Invalid email id!!(no uppercase allowed)");
            break;
            case 3:printf("Invalid email id!!(more than one '@' present)");
            break;
            case 4:printf("Invalid email id!!('.com' not present)");
            break;
            case 5:printf("Invalid email id!!(not ending with '.com')");
            break;
            case 6:printf("Invalid email id!!(no char between '@' and '.com')");
            break;
            case 7:printf("Invalid email id!!(duplicate email found)");
            break;
        }
       
    }
    strcpy(addressBook->contacts[addressBook->contactCount].email,email);
    addressBook->contactCount++;    //increase count of contact after getting valid details
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int matching_index[100]={0};
    int found=0;
    
    if(find_matching(addressBook,matching_index,&found))
    {
        disp_heading();
        for(int j=0;j<found;j++)
        {
            disp_i(addressBook,matching_index[j]);  //display all the matching contacts in the list
        }
    }
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    int matching_index[100]={0};
    int found=0;
    int e_index;
    while(find_matching(addressBook,matching_index,&found))
    {       
            printf("\nEnter 0 to exit\n");
            printf("\n----------------------------------------------------------------------------------\n");
            printf("| %-8s | %-18s | %-18s | %-25s |\n","Sl no.","Name","Phone no.","Email");
            printf("----------------------------------------------------------------------------------\n"); 
            for(int j=0;j<found;j++)
            {
                printf("| %-8d | %-18s | %-18s | %-25s |\n",j+1,addressBook->contacts[matching_index[j]].name,addressBook->contacts[matching_index[j]].phone,addressBook->contacts[matching_index[j]].email);
                printf("----------------------------------------------------------------------------------\n"); 
            }
            printf("\nEnter the contact sl no. to edit\n");
            scanf("%d",&e_index);

            if(!e_index)
            break;

            if(e_index<1||e_index>found)
            {
                printf("Invalid sl no.!!\n");   //check sl no in list
                continue;
            }
            
            edit_index(matching_index[e_index-1],addressBook);  //edit the contact at i'th index
            break;
    } 
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
       
    int matching_index[100]={0};
    int found=0;
    int d_index;
    while(find_matching(addressBook,matching_index,&found))
    {       
            printf("\nEnter 0 to exit.\n");
            printf("\n----------------------------------------------------------------------------------\n");
            printf("| %-8s | %-18s | %-18s | %-25s |\n","Sl no.","Name","Phone no.","Email");
            printf("----------------------------------------------------------------------------------\n"); 
            for(int j=0;j<found;j++)
            {
                printf("| %-8d | %-18s | %-18s | %-25s |\n",j+1,addressBook->contacts[matching_index[j]].name,addressBook->contacts[matching_index[j]].phone,addressBook->contacts[matching_index[j]].email);
                printf("----------------------------------------------------------------------------------\n"); 
            }
          
            printf("\nEnter the contact sl no. to delete\n");
            
            scanf("%d",&d_index);
            if(!d_index)
            break;
            if(d_index<1||d_index>found)
            {
                printf("Invalid sl no.!!\n");   //entered sl no. in list
                continue;
            }
            
            delete_index(matching_index[d_index-1],addressBook);    //delete the contact at entered index
            break;
    }    
}

//validate_name
int validate_name(char *name,AddressBook *addressBook)
{
    if(strlen(name)<2)  //check whether length of input less than 2
    {
        return 1;
    }
    for(int i=0;name[i]!='\0';i++)
    {
        if(!(isalnum(name[i]) || name[i]==' ')) //check whether the input contact only char and space
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
int validate_email(char email[],AddressBook *addressBook)
{
    int at_count=0;
    char *at_address;
    
    if(!isalnum(email[0]))
    return 1;       //cheking whether first char is alnum

    for(int i=0;i<strlen(email);i++)
    {
        if(email[i]>='A'&&email[i]<='Z') 
        return 2;               //check for uppercase

        if(email[i]=='@')
        {
            at_count+=1;
            at_address=&email[i];      //counting '@' count and storing its address
        }
    }

    if(at_count!=1)
    return 3;           //checking '@' occurs only one time

    char *dot_address;
    dot_address=strstr(email,".com");   //finding the address of ".com"

    if(dot_address==NULL)   //whether .com present or not
    return 4;

    if(&email[strlen(email)-4]!=dot_address)
    return 5;           //check last 4 char '.com'

    if(at_address+1==dot_address)
    return 6;           // checking a character is present between '@' and '.com'

    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(!strcmp(email,addressBook->contacts[i].email))
        return 7;           //check for duplicate email
    }

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

void delete_index(int i,AddressBook *addressBook)   //
{
    //delete the contact at i'th index

    for(int j=i;j<addressBook->contactCount-1;j++)
    {
        addressBook->contacts[j]=addressBook->contacts[j+1];
    }
    addressBook->contactCount--;
}

void disp_heading() //display heading
{
    //display heading of the contact list table 
    printf("\n--------------------------------------------------------------------\n");
    printf("| %-18s | %-18s | %-22s |\n","Name","Phone no.","Email");
    printf("--------------------------------------------------------------------\n"); 
}

void disp_i(AddressBook*addressBook,int i)  //display contact at i'th the index
{
     //display the contact at i'th index
    printf("| %-18s | %-18s | %-22s |\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    printf("--------------------------------------------------------------------\n");
}

int find_matching(AddressBook *addressBook,int matching_index[],int *found) //return array containing all the matching conctact indexes
{
    //saving matching contact index to array
    
    char input[60];
    int criteria;
    
    printf("Enter search criteria\n1.Name\n2.Phone\n3.email\n");
    scanf("%d",&criteria);
    
    printf("Enter criteria detail of the contact\n");
    scanf(" %[^\n]",input);

    switch(criteria)
    {
        case 1:
        //finding index in contacts which match i/p name
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strcasestr(addressBook->contacts[i].name,input)) 
                    {
                        matching_index[*found]=i;   //saving to array the index of matched contact
                        (*found)++;
                        
                    }
            }
            break;

        case 2:
        //finding index in contacts which match i/p phone
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strcasestr(addressBook->contacts[i].phone,input))
                    {
                        matching_index[*found]=i;   //saving to array the index of matched contact
                        (*found)++;
                    
                    }
            }
            break;

        case 3:
        //finding index in contacts which match i/p email
            for(int i=0;i<addressBook->contactCount;i++)
            {
                if(strcasestr(addressBook->contacts[i].email,input))
                    {
                        matching_index[*found]=i;   //saving to array the index of matched contact
                        (*found)++;
                     
                    }
            }
            break;

        default:
            printf("Contact not found!!\n");
            return 0;
    }
    return 1;
}

void edit_index(int i,AddressBook*addressBook)  //edit the given index
{
    int flag=0;
    while(!flag)
    {
        int criteria;
        char edit[50];
        
        printf("\nEnter what to edit\n");
        printf("1.Name\n2.phone\n3.email\n");   //input what to edit
        scanf("%d",&criteria);

        if(criteria<1||criteria>3)
        {
            printf("Inavlid!!\n");
            continue;
        }
    
        printf("Enter the edit(new value)\n");  //input new value
        scanf(" %[^\n]",edit);

        switch(criteria)        //editing according to criteria
        {
            //add validation also
            case 1:if(!validate_name(edit,addressBook))
                {
                    strcpy(addressBook->contacts[i].name,edit);     //validate name before editing
                    flag=1;
                }
                else
                printf("Invalid input\n");

                break;
                
            case 2:if(!validate_phone(edit,addressBook))
                {
                    strcpy(addressBook->contacts[i].phone,edit);        //validate phone num before editing
                    flag=1;
                }
                else
                printf("Invalid input\n");

                break;

            case 3:if(!validate_email(edit,addressBook))
                {
                    strcpy(addressBook->contacts[i].email,edit);        //validate email before editing
                    flag=1;
                }
                else
                printf("Invalid input\n");

                break;
        }
    }

}

