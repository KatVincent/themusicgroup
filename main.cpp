/* 
 * File:   main.cpp
 * Author: Kat Vincent
 *
 */

#include <cstdlib>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream> 
#include <random>
#include "MyQueue.h"

using namespace std;

struct Work
{
    int    ID ; 
    string Employee ; 
} ;


string GetEmployeeName(void) ; 

//Let's initialise our queue to 15 spaces max.
MyQueue<Work> MainQueue(15) ; 

bool NewItem       ;
int  LoopCounter   ; 
int  JobsCompleted ;
int  ReadWorkItem  ;
int  JobNumber     ; 
Work ReadWorkJob   ;
Work NewWorkJob    ; 
int  NameCounter   ;



/*------------------------------------------------------------------------------
 
 Function Name  :       WriteItems
 
 Description    :       This is the writer thread that is responsible
                        for attempting to push new work tasks on to the queue.
 
                        It will call the queue's "Push" function, to check
                        if there is space for the latest work task. 
 
 Parameters     :       None
 
 Return         :       None
 
 -----------------------------------------------------------------------------*/
void WriteItems(void)
{
    while(true)
    {
        if (NewItem == true)
        {

            //Let's set up a random number generator for our work tasks
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 100);


            NewWorkJob.ID = dis(gen) ; 
            NewWorkJob.Employee = GetEmployeeName() ; 
            
            if (MainQueue.Push(NewWorkJob))
            {
                std::cout << "task ID:"  <<  NewWorkJob.ID << " pushed \n";
                NewItem    = false ;
                JobNumber++ ; 
            }
            else
            {
                //std::cout << "No space for our task\n";
            }
            
        }
        else
        {
            //nothing to do as we have no item to try and push
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));  
    }
    
    
}

/*------------------------------------------------------------------------------
 
 Function Name  :       ReadItems
 
 Description    :       This is the reader thread that is responsible
                        for attempting to read new work tasks from the queue.
 
                        It will call the queue's "Pop_Try" function, to check
                        if there are any available tasks, and if there are
                        those will be returned, where it could then work on 
                        them as needed. 
 
 Parameters     :       None
 
 Return         :       None
 
 -----------------------------------------------------------------------------*/
void ReadItems(void)
{
    bool Result ; 
    while(true)
    { 
        ReadWorkJob = MainQueue.Pop_Try(&Result) ; 
        if (Result)
        {
            std::cout << "task for " << ReadWorkJob.Employee  << " ID:" <<  ReadWorkJob.ID << " popped successfully\n" ;

            JobsCompleted++ ;  
            
            //Do work on the job here.
        }
        else
        {
            //Nothing to read so nothing to work on.
        }
        

        std::this_thread::sleep_for(std::chrono::milliseconds(11));  
    }
    
}


/*------------------------------------------------------------------------------
 
 Function Name  :       GetEmployeeName
 
 Description    :       This function will return a string name from a list,
                        so new work jobs can be simulated. 
 
 Parameters     :       None
 
 Return         :       A name as a string.
 
 -----------------------------------------------------------------------------*/
string GetEmployeeName(void)
{
    string Name ; 
    
    Name = "" ; 
    
    switch (NameCounter)
    {
        case 0:
            Name = "Alice" ; 
            break ;
        case 1:
            Name = "Bob" ; 
            break ;
        case 2:
            Name = "Claire" ; 
            break ;
        case 3:
            Name = "Dan" ; 
            break ;
        case 4:
            Name = "Ellen" ; 
            break ;
        case 5:
            Name = "Frank" ; 
            break ;
        case 6:
            Name = "Gill" ; 
            break ;
        case 7:
            Name = "Howard" ; 
            break ;
        case 8:
            Name = "Iris" ; 
            break ;
        case 9:
            Name = "Jack" ; 
            break ;
    }
    
    NameCounter++ ; 
    
    if (NameCounter > 10)
    {
        NameCounter = 0 ; 
    }
    
    return Name ; 
}


/*------------------------------------------------------------------------------
 
 Function Name  :       main
 
 Description    :       This is the main function that will initialise
                        the two threads. It will also simulate jobs
                        "arriving" for the writer thread to push.

 Parameters     :       N/A
 
 Return         :       N/A
 
 -----------------------------------------------------------------------------*/
int main(int argc, char** argv) 
{
    //For now, no new items to add
    NewItem       = false ; 
    
    LoopCounter   = 0 ;
    JobsCompleted = 0 ; 
    JobNumber     = 0 ;
    
    //Let's also initialise our two threads.
    std::thread Writer (WriteItems);
    std::thread Reader (ReadItems) ;
    

    //The aim of this loop is to simulate jobs being added to the system,
    //so they can be pushed and popped. 
    //To keep things simple, the number of work jobs that will be added will
    //be five. 
    while (JobsCompleted < 5)
    {
        //this will be used a flag for introducing a new task. 
        LoopCounter++ ;

        if (LoopCounter == 20)
        {
            //OK, let's add a new one.  
            NewItem     = true ; 
            LoopCounter = 0    ; 
        }
    }
    
    //We want to detach our threads now that we're happy with the tasks so far. 
    Writer.detach();
    Reader.detach();


    return 0;
}
