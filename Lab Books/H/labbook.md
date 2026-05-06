# Explaining PersonNode

**AddressBookSLL** is a friend class of **PersonNode** class. 
  So that it can access private members. 

  Added Getters - getName() and setAge() - made them const (they do not modify the values)
  Added Setters - setName() and setAge()

### PersonNode.h

<img width="705" height="440" alt="Screenshot 2026-03-19 at 18 16 42" src="https://github.com/user-attachments/assets/e5096e68-d0d4-4745-be1d-5bc035d46b55" />

PersonNode.cpp implements the functionality of methods declared in PersonNode.h
### PersonNode.cpp

<img width="392" height="752" alt="Screenshot 2026-03-19 at 18 30 43" src="https://github.com/user-attachments/assets/2cf74560-7827-4c36-b4e5-72d9e7b4c5c3" />


# Explaining AddressBookSLL

This class initiates more functionality than **PersonNode** class. 
It defines: 
  - AddPerson() - Updates pointers, adds new node. 
  - FindPerson() - which is of type PersonNode*. 
  - DeletePerson() - boolean return type, updates pointers, removes users from address book
  - Print() (to ostream)
### AddressBookSLL.h
<img width="514" height="486" alt="Screenshot 2026-03-19 at 18 27 06" src="https://github.com/user-attachments/assets/083a4eef-b5ed-4c5d-b2a4-6c69b1a2b293" />

### AddressBookSLL.cpp
<img width="430" height="92" alt="Screenshot 2026-03-19 at 18 32 06" src="https://github.com/user-attachments/assets/e3f17044-23ab-4d63-b18a-b2ef6fad2585" />
This declares that **m_head** is the end of the linked list

<img width="384" height="304" alt="Screenshot 2026-03-19 at 18 33 01" src="https://github.com/user-attachments/assets/1130e9a8-d378-441e-b5b8-bb38f61551dc" />
Destructor has been modified from the original

<img width="539" height="602" alt="image" src="https://github.com/user-attachments/assets/727253e2-ab54-47f3-808d-3ac3fe427c75" />
Methods for AddPerson & FindPerson

### How AddPerson() works
We create a new instance of PersonNode (new element / person to add in the list).
If the list is empty, then m_head points to the newNode. 
If the list is not empty, We traverse to the element which has a **nullptr**, add element, update the pointers. 

### How FindPerson() works
Gets current node (m_head)
if list is not empty, return the current node
else return nullptr (list is empty. 


<img width="411" height="565" alt="image" src="https://github.com/user-attachments/assets/7ec56e08-72be-44a9-b12d-b528a0f91936" />
Method to delete a person. 
This method uses previous and current pointers and updates them. It also check whether we are at the first node, node in between first and last. 


<img width="536" height="243" alt="image" src="https://github.com/user-attachments/assets/48c17fcf-2813-4697-b1df-11340c4646da" />

Print method

### Main.cpp
<img width="285" height="413" alt="image" src="https://github.com/user-attachments/assets/6434c986-fa08-4044-b76f-156f3c708ebe" />


