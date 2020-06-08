#include<stdio.h>
#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }  

  if (mysql_real_connect(con, "localhost", "ccnteam", "Ccn123!@#", 
          "pollbooth", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }
  
  if (mysql_query(con, "DROP TABLE IF EXISTS Voting")) {
      finish_with_error(con);
  }
  
  if (mysql_query(con, "CREATE TABLE Voting(username TEXT, aadharno TEXT, Candidate TEXT)")) {      
      finish_with_error(con);
  }
  mysql_close(con);
  exit(0);
}
