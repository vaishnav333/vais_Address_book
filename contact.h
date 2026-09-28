#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;
int find_i(AddressBook*addressBook,char input[],int criteria_s);
void disp_i(AddressBook*addressBook,int i);
void delete_index(int,AddressBook*addressBook);

void sort_name(AddressBook *addressBook);
void sort_phone(AddressBook *addressBook);
void sort_email(AddressBook *addressBook);

int validate_name(char *name);
int validate_phone(char phn[],AddressBook *addressBook);
int validate_email(char email[]);


void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);


#endif
