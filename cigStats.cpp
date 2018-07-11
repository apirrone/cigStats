#include <iostream>
#include <time.h>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CURRENCY "€"
#define PRICE_OF_PACK 11
#define PACKS_PER_WEEK 1

typedef struct tm tm;

bool checkDate(tm date){
  int then = mktime(&date);
  time_t now = time (NULL);

  return (now-then)>0;
}


tm readFile(char* file){

  tm date;
    
  if(access(file, F_OK)){// File does not exist  else
    std::cout << "ERROR : file does not exist" << std::endl;
    return date;
  }
  
  std::string line;
  std::ifstream myFile(file);
  
  if(myFile.is_open()){

    getline(myFile, line);
    date.tm_year = atoi(line.c_str());
    
    getline(myFile, line);
    date.tm_mon = atoi(line.c_str());
    
    getline(myFile, line);
    date.tm_mday = atoi(line.c_str());
    
    getline(myFile, line);
    date.tm_hour = atoi(line.c_str());
    
    date.tm_min=0;
    date.tm_sec=0;
    
    myFile.close();
    return date;
    
  }
  else{
    std::cout << "ERROR : could not read file" << std::endl;
    return date;
  }
}

void writeFile(tm date, char* filename){

  std::ofstream myFile(filename);
  if(myFile.is_open()){
    
    myFile << date.tm_year << "\n";
    myFile << date.tm_mon  << "\n";
    myFile << date.tm_mday << "\n";
    myFile << date.tm_hour << "\n";    
    
    myFile.close();
    
  }
  else
    std::cout << "ERROR : could not write file" << std::endl;  
}



time_t getDateInSeconds(tm date){
  return mktime(&date);
}

void setup(char* file){

  time_t time;
  tm date;
  int input;
  bool dateOk = false;
  
  do{
    
    puts("Year ? : ");
    scanf("%d", &input);
    date.tm_year=input-1900;

    puts("Month ? : ");
    scanf("%d", &input);
    date.tm_mon=input-1;

    puts("Day ? : ");
    scanf("%d", &input);
    date.tm_mday=input;

    puts("Hour ? : ");
    scanf("%d", &input);
    date.tm_hour=input;

    date.tm_min=0;
    date.tm_sec=0;

    dateOk = checkDate(date);
    
    if(!dateOk)
      std::cout << "Invalid date ! The date entered cannot be in the future" << std::endl;
    
  } while(!dateOk);
    
  writeFile(date, file);

  std::cout << "Setup Done !" << std::endl;
}

void display(char* file){
  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  
  tm date = readFile(file);  
  int start = mktime(&date); // date in seconds elapsed since epoch
  time_t seconds = time (NULL);
  
  float hours = ((seconds - start)/60.)/60.;
  float minutes = (hours - (long)hours)*60;
  
  int daysDisplay = (int)(hours/24);
  int heuresDisplay = (((int)(hours))%24);
  
  int months = (int)(daysDisplay/30); // heuristic
  int remainingDays = daysDisplay%30;

  int years = (int)(months/12);
  int remainingMonths = months%12;
  
  std::cout << "----------------------------------------" << std::endl;

  
  if(years>0){    
    std::cout << years;
    
    if(years > 1)
      std::cout << " years, ";
    else
      std::cout << " year, ";

    
    std::cout << remainingMonths;
    if(remainingMonths>1)
      std::cout << " months, ";
    else
      std::cout << " month, ";    
  }
  else{
    std::cout << months;    
    if(months>1)
      std::cout << " months, ";
    else
      std::cout << " month, "; 
  }
  std::cout << remainingDays;

  if(remainingDays>1)
    std::cout << " days " ;
  else
    std::cout << " day " ;
  
  std::cout << "and ";
  
  std::cout << heuresDisplay ;

  if(heuresDisplay>1)
    std::cout << " hours ";
  else
    std::cout << " hour ";
    
  
  std::cout << "since last cigarette (2018 may 23 at 22h)" << std::endl;

  int hoursInWeek = 24*7;
  int pricePerWeek = PRICE_OF_PACK*PACKS_PER_WEEK;
  float pricePerHour = pricePerWeek/(hoursInWeek*1.);
  float saved = hours*pricePerHour;

  std::cout << saved << CURRENCY << " saved " << std::endl;

  
  std::cout << "----------------------------------------" << std::endl;  
}

void usage(){
  std::cout << "USAGE : ./cigStats" << std::endl;
  std::cout << "OR" << std::endl;
  std::cout << "USAGE : ./cigStats --setup" << std::endl;
}

int main(int argc, char* argv[]){
  
  char* file = (char*)".cigStatsConfig"; // to avoid annoying warning (depreciated conversion from string constant to char*)
  
  if(argc > 2){
    usage();
    return 0;
  }

  if(argc == 2){
    if(strcmp(argv[1], "--setup") !=0){
      usage();
      return 0;
    }
    else
      setup(file);
  }

  display(file);

  
  return 1;
}
