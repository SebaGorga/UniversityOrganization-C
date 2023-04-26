/***********************************************
*
* @Purpose: to develop a support tool that can assign students to specific classrooms according to the degree they wish to study
* @Author: Sebastián Félix Gorga
* @Creation date: 12/05/2022
* @Date of last modification: 15/05/2022
*
************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linkedlist.h"


typedef struct {
 char name[MAX_STRING_LENGTH];
 int current_capacity;
 LinkedList students;
} Classroom;

typedef struct {
 char name[MAX_STRING_LENGTH];
 int num_classrooms;
 Classroom *classrooms;
} Degree;

typedef struct {
 int num_degrees;
 Degree *elements;
} Degrees;

/***********************************************
*
* @Purpose: separate the information coming from the string that contains the degree information, save it to the structs and 
            allocate a space in the memory for the Classroom structs
* @Parameters: in: degree_str = the string containing the information
               in: all_degrees = the initial struct with the pointer to all the memory
               in: i = index that refers to which space of the degree memory allocation the info should go to
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees separateStringDegree (char degree_str[MAX_STRING_LENGTH], Degrees all_degrees, int i) {

  int j = 0, k = 0, l = 0;
  char num_classrooms_str[MAX_STRING_LENGTH];
  
  while (degree_str[j] != ' ') {                             //this loop separates the number of classrooms 
    num_classrooms_str[k] = degree_str[j];
    j++;
    k++;
    
    }
  j++;
  
  num_classrooms_str[k] = '\0';
  all_degrees.elements[i].num_classrooms  = atoi (num_classrooms_str);              //turn the string into an integer and 
                                                                                    //save it to the struct

  //allocate in the memory space for the Classroom structs equal to the number of classrooms the degree has
  all_degrees.elements[i].classrooms = (Classroom*) malloc (sizeof(Classroom) * all_degrees.elements[i].num_classrooms);

  while (degree_str[j] != '\0') {                            //this loop separates the degree name and saves it to the struct

    all_degrees.elements[i].name[l] = degree_str[j];
    j++;
    l++;
    
    }
  all_degrees.elements[i].name[l] = '\0';

  return all_degrees;
}

/***********************************************
*
* @Purpose: separate the information coming from the string that contains the classroom information, save it to the structs and 
            create the LinkedLists that will allocate the students
* @Parameters: in: classroom_str = the string containing the information
               in: all_degrees = the initial struct with the pointer to all the memory
               in: i = index that refers to which space of the degree memory allocation the info should go to
               in: j = index that refers to which space of the classroom memory alllocation the info should go to
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees separateStringClassroom (char classroom_str[MAX_STRING_LENGTH], Degrees all_degrees, int i, int j) {

  int k = 0, l = 0;
  
  while (classroom_str[k] != '\0') {                     //this loop separates the classroom name and saves it to the struct

    all_degrees.elements[i].classrooms[j].name[l] = classroom_str[k];
    k++;
    l++;
    
    }
  all_degrees.elements[i].classrooms[j].name[l] = '\0';
 

  //create the Linked List for the class
  all_degrees.elements[i].classrooms[j].students = LINKEDLIST_create ();

  all_degrees.elements[i].classrooms[j].current_capacity = 0;   //set the current capacity of each classroom to 0 until the 
                                                                //students are loaded
  
  return all_degrees;
}

/***********************************************
*
* @Purpose: open the file containing the degrees and classrooms information and read its contents
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory             
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees loadDegrees (Degrees all_degrees) {

  FILE *fp;
  char filename[MAX_STRING_LENGTH];
  int i, j;
  char number_degrees[MAX_STRING_LENGTH];
  char degree_str[MAX_STRING_LENGTH];
  char classroom_str[MAX_STRING_LENGTH];
  int ctr_file_class = 0;
  
  
  while (ctr_file_class == 0){                                    //this loop makes sure that the 
                                                                  //correct class file is loaded
    printf ("\nType the name of the 'classrooms' file: ");
    scanf ("%s", filename);
    
    fp = fopen (filename, "r");
  
    if (NULL == fp) {
    
      printf ("\nERROR: Can't open file '%s'\n", filename);        //message the user receives in case that 
                                                                  //the name of the file is incorrect
      
    }
    else {
      
      ctr_file_class = 1;                                        //this ends the loop for loading the class file
      
      fgets (number_degrees, MAX_STRING_LENGTH, fp);             //first read the initial number that contains the number of degrees
      number_degrees [ strlen(number_degrees) -1 ] = '\0';
      all_degrees.num_degrees = atoi (number_degrees);           //turn the string into a number and save it to the struct

      //allocate space in the memory for the Degree structs equal to the number of degrees there are
      all_degrees.elements = (Degree*) malloc (sizeof(Degree) * all_degrees.num_degrees);
  
        for (i = 0; i < all_degrees.num_degrees; i++) {           //this loop reads the degree names and number of classes
          
          fgets (degree_str, MAX_STRING_LENGTH, fp);
          degree_str [ strlen(degree_str) -1 ] = '\0';
    
          all_degrees = separateStringDegree (degree_str, all_degrees, i); //this function saves the degreee names and number of classes
          
          for (j = 0; j < all_degrees.elements[i].num_classrooms; j++) { //this loop reads the classroom names and maximum 
                                                                        //capacity of students
            
            fgets (classroom_str, MAX_STRING_LENGTH, fp);
            if (classroom_str [ strlen(classroom_str) -1 ] == '\n'){     
              classroom_str [ strlen(classroom_str) -1 ] = '\0';
            }
            all_degrees =  separateStringClassroom(classroom_str, all_degrees, i, j); //this function saves the 
                                                                                      //classroom names
            
          }
          
        } 
  
      fclose (fp);                                                 //close the file
        
    }
  }
  return all_degrees;
}

/***********************************************
*
* @Purpose: destroy the memory once the user has finished with the program 
* @Parameters:  in: all_degrees = the initial struct with the pointer to all the memory
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees freeAll (Degrees all_degrees) {

  int i, j;
  
  for (i = 0; i < all_degrees.num_degrees; i++){

    for (j = 0; j < all_degrees.elements[i].num_classrooms; j++){

      LINKEDLIST_destroy (&all_degrees.elements[i].classrooms[j].students); //first destroy the LinkedLists that contain the students
      
    }
    if (all_degrees.elements[i].classrooms != NULL){
      free (all_degrees.elements[i].classrooms);                    //then free the pointers that have the Classroom structs
    }
  }
  if (all_degrees.elements != NULL){
    free (all_degrees.elements);                                     //finally free the initial pointer to the degrees
  }
  all_degrees.elements = NULL;                                     //make sure the pointer points to NULL

   return all_degrees;
}

/***********************************************
*
* @Purpose: separate the information coming from the string that contains the student information and save it to the LinkedLists
* @Parameters: in: student = the string containing the information with the student's name and degree
               in: student_login = the string containing the information with the student's login
               in: all_degrees = the initial struct with the pointer to all the memory
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees separateStringStudent (char student[MAX_STRING_LENGTH],  char student_login[MAX_STRING_LENGTH], Degrees all_degrees){

  char student_class[MAX_STRING_LENGTH];
  int i = 0, j = 0, k = 0, l;
  int info_saved = 0;
  Student temp_student; 
  
  while (student[i] != ' '){                            //this loop separates the student's name and saves it to the temporary struct

    temp_student.name[j] = student[i];
    i++;
    j++;

  }
  i++;
  temp_student.name[j-1] = '\0';

  strcpy(temp_student.login, student_login);             //copy the login in the temporary student struct

  while (student[i] != '\0'){                           //this loop separates the student's class

    student_class[k] = student[i];
    i++;
    k++;

  }
  student_class[k] = '\0';

  for (l = 0; l < all_degrees.num_degrees; l++){

    //this if makes sure that the student is allocated in the degree that matches
    if (strcmp(student_class, all_degrees.elements[l].name) == 0){

      //this loop prevents that the student's information is saved more than once
      while (info_saved == 0){

        LINKEDLIST_add (all_degrees.elements[l].classrooms[0].students, temp_student); //save the student in the LinkedList
  
        all_degrees.elements[l].classrooms[0].current_capacity ++;                    //increase the classroom's current capacity
        info_saved = 1;
            
        }
      
    }
 
  }

  return all_degrees;
}

/***********************************************
*
* @Purpose: open the file containing the students' information and read its contents
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory             
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees loadStudents (Degrees all_degrees) {

  FILE *fp2;
  int ctr_file_stud = 0;
  char filename[MAX_STRING_LENGTH];
  char student[MAX_STRING_LENGTH];
  char login_student[MAX_STRING_LENGTH];
  
  while (ctr_file_stud == 0){                                  //this loop makes sure that the 
                                                              //correct students file is loaded

    printf ("\nType the name of the 'students' file: ");
    scanf ("%s", filename);
    
    fp2 = fopen (filename, "r");

    if (NULL == fp2) {
    
      printf ("\nERROR: Can't open file '%s'\n", filename);    //message the user receives in case that 
                                                              //the name of the file is incorrect
      
    }
    else {

      ctr_file_stud = 1;                                      //this ends the loop for loading the students file
      
      fgets (student, MAX_STRING_LENGTH, fp2);                //do an initial reading of the first line in the file
      student [ strlen(student) -1 ] = '\0';
  
        while (!feof(fp2)) {                                  //this loop ends when we have reached the end of the file

          fgets (login_student, MAX_STRING_LENGTH, fp2);      //read the student's login
          if (login_student [ strlen(login_student) -1 ] == '\n'){
            login_student [ strlen(login_student) -1 ] = '\0';
          }

          all_degrees = separateStringStudent (student, login_student, all_degrees); //this function saves the student's information
                                                                                     //to the LinkedLists
          
          fgets (student, MAX_STRING_LENGTH, fp2);
          student [ strlen(student) -1 ] = '\0';
            
        } 
  
      fclose (fp2);                                          //close the file
  
    }
    
  }

  return all_degrees;
}

/***********************************************
*
* @Purpose: show almost all of the information stored in the memory
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory             
* @Return: ----.
*
************************************************/

void functionSummary (Degrees all_degrees) {

  int i, j;
  
  for (i = 0; i < all_degrees.num_degrees; i++){

    printf ("\n%s\n", all_degrees.elements[i].name);          //first print the degree's names

    for (j = 0; j < all_degrees.elements[i].num_classrooms; j++){

      //then print the degree's classrooms and their current capacity
      printf ("%s %d/inf\n", all_degrees.elements[i].classrooms[j].name, all_degrees.elements[i].classrooms[j].current_capacity);
      
    }
    
  }
  
}

/***********************************************
*
* @Purpose: check that the degree the user wants to access actually exists
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory
               in: degree = the string with the name of the degree the user wants to access
* @Return: returns an index that indicates whether the degree exists or not
*
************************************************/

int checkDegreeExists (Degrees all_degrees, char degree[MAX_STRING_LENGTH]) {

  int index = 0;
  int i;

  for (i = 0; i < all_degrees.num_degrees; i++){                     //this loop checks that the name the user has written exists 
                                                                    //in the information saved and returns a signal in an index

    if (strcmp (all_degrees.elements[i].name, degree) == 0){

      index = 1;
      
    }

  }
  
  return index;
  
}

/***********************************************
*
* @Purpose: check where the degree the user wants to access is
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory
               in: degree = the string with the name of the degree the user wants to access
* @Return: returns an index that indicates where the degree is located in the memory
*
************************************************/

int findDegreeIndex (Degrees all_degrees, char degree[MAX_STRING_LENGTH]) {

  int index;
  int i;

  for (i = 0; i < all_degrees.num_degrees; i++){                    //this loop checks where the degree 
                                                                    //requested by the user is allocated

    if (strcmp (all_degrees.elements[i].name, degree) == 0){

      index = i;
      
    }

  }

  return index;
  
}

/***********************************************
*
* @Purpose: show the information specific to the degree the user wants
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory             
* @Return: ----.
*
************************************************/

void functionShowDegree (Degrees all_degrees) {

  int i, k;
  int index_degree;
  char degree[MAX_STRING_LENGTH];
  char garbage;
  Student temp_student;

  scanf ("%c", &garbage);                                        //this takes the '\n' from the user input
  printf ("\nDegree to show? ");
  fgets (degree, MAX_STRING_LENGTH, stdin);                      //this reads what is the degree the user wants to show
  degree [ strlen(degree) -1 ] = '\0';

  if (!checkDegreeExists(all_degrees, degree)){

    printf ("\nERROR: Can't find degree");                      //message the user receives in case the degree
                                                                //does not exist in the database
 
  }
  else {

    index_degree = findDegreeIndex (all_degrees, degree);

    //this loop resets the point of view of the desired degree to be displayed to the beginning of the LinkedList
    for (k = 0; k < all_degrees.elements[index_degree].num_classrooms; k++){
      if (!LINKEDLIST_isEmpty (all_degrees.elements[index_degree].classrooms[k].students)){
        LINKEDLIST_goToHead (all_degrees.elements[index_degree].classrooms[k].students);
      }
    }

    for (i = 0; i < all_degrees.elements[index_degree].num_classrooms; i++){
      
      if (!LINKEDLIST_isEmpty (all_degrees.elements[index_degree].classrooms[i].students)){

        while(!LINKEDLIST_isAtEnd (all_degrees.elements[index_degree].classrooms[i].students)){

          //first save the element containing the student's information to the temporary struct
          temp_student = LINKEDLIST_get (all_degrees.elements[index_degree].classrooms[i].students);
          //first print the student's name, then their login and finally in which classrom they are appointed
          printf ("\n%s (%s): %s", temp_student.name, temp_student.login, all_degrees.elements[index_degree].classrooms[i].name);
          //go to the next node
          LINKEDLIST_next (all_degrees.elements[index_degree].classrooms[i].students);
        }
      
      }
    }
    
  }

  printf ("\n");
  
}

/***********************************************
*
* @Purpose: separate the information coming from the string that contains the student information and save it to the structs 
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory
               in: index_degree = index that refers to which space of the degree memory allocation the info is
               in: index_classroom = index that refers to which space of the classroom memory allocation the info is
               in: index_where_to = index that refers to where the information of the student should go to
               in: login = the string containing the login of the student that the user wants to move
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees relocateStudent (Degrees all_degrees, int index_degree, int index_classroom, int index_where_to, char login[MAX_STRING_LENGTH]){

  Student temp_student;

  //this makes sure that the POV of the classroom where we want to send the student is right at the end 
  if (!LINKEDLIST_isEmpty (all_degrees.elements[index_degree].classrooms[index_where_to - 1].students)){
    
    while (!LINKEDLIST_isAtEnd (all_degrees.elements[index_degree].classrooms[index_where_to - 1].students)) {
    
      LINKEDLIST_next (all_degrees.elements[index_degree].classrooms[index_where_to - 1].students);
    
    }
    
  }

  //this sets the POV of the LinkedList where the student currently is to the beginning 
  LINKEDLIST_goToHead (all_degrees.elements[index_degree].classrooms[index_classroom].students);

  //this loop checks at which POV the student is at
  while (!LINKEDLIST_isAtEnd (all_degrees.elements[index_degree].classrooms[index_classroom].students)) {

    //save the student to the temprary struct
    temp_student = LINKEDLIST_get (all_degrees.elements[index_degree].classrooms[index_classroom].students);

    //once we find the correct POV, relocate the student by adding them to the other classroom 
    //and then removing them from the current one. Also increase the current capacity of the classrooms accordingly.
    if (strcmp(temp_student.login, login) == 0) {
      
      LINKEDLIST_add (all_degrees.elements[index_degree].classrooms[index_where_to - 1].students, temp_student);
      LINKEDLIST_remove (all_degrees.elements[index_degree].classrooms[index_classroom].students);
      all_degrees.elements[index_degree].classrooms[index_where_to - 1].current_capacity ++;
      all_degrees.elements[index_degree].classrooms[index_classroom].current_capacity --;
      
    }

    //go to the next node
    LINKEDLIST_next (all_degrees.elements[index_degree].classrooms[index_classroom].students);
    
  }
  
  return all_degrees;
  
}

/***********************************************
*
* @Purpose: obtain the information regarding which student the user wants to move and move him/her
* @Parameters: in: all_degrees = the initial struct with the pointer to all the memory             
* @Return: returns the initial static structure with the pointer to all the memory
*
************************************************/

Degrees functionMoveStudent (Degrees all_degrees){

  int i, k, m;
  int index_degree;
  int login_exists = 0;
  int index_classroom;
  int index_where_to;
  char degree[MAX_STRING_LENGTH];
  char login[MAX_STRING_LENGTH];
  char garbage;
  Student temp_student;

  scanf ("%c", &garbage);
  printf ("\nDegree? ");
  fgets (degree, MAX_STRING_LENGTH, stdin);                  //first read the degree where the student that 
                                                            //the user wants to move is located
  degree [ strlen(degree) -1 ] = '\0';

  //if the degree is one that actually exists in the database, do the following
  if (checkDegreeExists(all_degrees, degree)){

    //find where the degree is allocated in the memory
    index_degree = findDegreeIndex (all_degrees, degree);

    printf ("\nClassrooms and capacity:\n");

    //this loop resets the point of view of the desired degree to be displayed to the beginning of the LinkedList
    for (m = 0; m < all_degrees.elements[index_degree].num_classrooms; m++){

      if (!LINKEDLIST_isEmpty (all_degrees.elements[index_degree].classrooms[m].students)){
        
        LINKEDLIST_goToHead (all_degrees.elements[index_degree].classrooms[m].students);
      
      }
    }

    //print the degree's classrooms, capacity and students (with their login)
    for (i = 0; i < all_degrees.elements[index_degree].num_classrooms; i++){

      printf ("%d. %s %d/inf\n", (i+1), all_degrees.elements[index_degree].classrooms[i].name, 
        all_degrees.elements[index_degree].classrooms[i].current_capacity);

      while (!LINKEDLIST_isAtEnd (all_degrees.elements[index_degree].classrooms[i].students)) {

        //save the student to the temporary struct
        temp_student = LINKEDLIST_get (all_degrees.elements[index_degree].classrooms[i].students);
        printf ("%s\n", temp_student.login);
        //go to the next node
        LINKEDLIST_next (all_degrees.elements[index_degree].classrooms[i].students);
        
      }
      
    }

    //this loop resets the point of view of the desired degree to be displayed to the beginning of the LinkedList
    for (m = 0; m < all_degrees.elements[index_degree].num_classrooms; m++){

      if (!LINKEDLIST_isEmpty (all_degrees.elements[index_degree].classrooms[m].students)){
        
        LINKEDLIST_goToHead (all_degrees.elements[index_degree].classrooms[m].students);
      
      }
    }

    printf ("\nWho do you want to move (login)? ");
    fgets (login, MAX_STRING_LENGTH, stdin);              //this reads who the user wants to move
    login [ strlen(login) -1 ] = '\0';

    //this loops check whether the login is correct and where the student's information is stored in the memory
    for (k = 0; k < all_degrees.elements[index_degree].num_classrooms; k++){

      if (!LINKEDLIST_isEmpty(all_degrees.elements[index_degree].classrooms[k].students)){
        
        while (!LINKEDLIST_isAtEnd (all_degrees.elements[index_degree].classrooms[k].students)) {
  
          //save the student to the temporary struct
          temp_student = LINKEDLIST_get (all_degrees.elements[index_degree].classrooms[k].students);

          if (strcmp(temp_student.login, login) == 0){

            index_classroom = k;
            login_exists = 1;
          
          }
          //go to the next node
          LINKEDLIST_next (all_degrees.elements[index_degree].classrooms[k].students);
        
        }      
      }
    }
    //if the login that the user input was one that coincides with the one in the databse do the following
    if (login_exists == 1){

      printf ("\nTo which classroom (index)? ");
      scanf ("%d", &index_where_to);                //get the classroom index where the user wants to move the student
      scanf ("%c", &garbage);

      //check whether: 1) the index is 0 or less, 2) the index surpases the number of classrooms, 
      //3) the user wants to move the student where he/she already is
      if (index_where_to < 1 || index_where_to > all_degrees.elements[index_degree].num_classrooms || 
        (index_classroom + 1) == index_where_to) {

        printf ("\nERROR: Can't move student"); //message the user receives if any of the previous mistakes happened

        printf ("\n");
        
      }
      else {

        //this function moves the student
        all_degrees = relocateStudent (all_degrees, index_degree, index_classroom, index_where_to, login);
        
      }
      
    }
      
    else {

      printf ("\nERROR: Can't move student");   //message the user receives if the login does not exist

      printf ("\n");
      
    }

  }
  else {

    printf ("\nERROR: Can't move student");      //message the user receives if the degree does not exist

    printf ("\n");
    
  }
  
  
  return all_degrees;
  
}

int main () {

  Degrees all_degrees;
  int ctr_menu = 0;
  int option;
  
  printf ("Welcome!\n");

  all_degrees = loadDegrees (all_degrees);      //this function reads the classrooms' file

  all_degrees = loadStudents (all_degrees);     //this function reads the students' file

  while (ctr_menu == 0){                        //while the user has not selected the exit option do the following

    //print the options' menu
    printf ("\n1. Summary | 2. Show degree students | 3. Move student | 4. Exit\n");
    printf ("Select option: ");
    scanf ("%d", &option);

    if (option < 1 || option > 4) {

      printf ("\nERROR: Wrong option number\n");  //message the user receives if the option selected is less than 1 or more than 4
      
    }
    else {

      switch (option){

        case 1: 

          functionSummary (all_degrees);

        break;

        case 2:

          functionShowDegree (all_degrees);

        break;

        case 3:

          functionMoveStudent (all_degrees);

        break;

        case 4:

        all_degrees = freeAll (all_degrees);    //when the user wants to exit erase the memory saved 
        ctr_menu = 1;                           //this ends the option menu loop
        printf ("\nBye!\n");               

      } 

    }

  }

  
  return 0;
}
