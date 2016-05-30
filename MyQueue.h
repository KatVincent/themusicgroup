/* 
 * File:   MyQueue.h
 * Author: Kat Vincent
 *
 */

#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "MyQueue.h"
#include <queue>
#include <thread>
#include <mutex>

template<class T>
class MyQueue 
{

private:
    std::queue<T> TheQueue;
    std::mutex QueueMutex ; 
    
    int MaxSize ; 
    
public:    
    MyQueue(int Length)
    {
        if (Length > 0)
        {
            MaxSize = Length ; 
        }
        else
        {
            //Default to 10
            MaxSize = 10 ; 
        }
    }

/*------------------------------------------------------------------------------
 
 Function Name  :       Push
 
 Description    :       This function will check if the stack is full, and
                        if it's not, it will take the passed item  and push 
                        it to the back of the queue. If that was successful, 
                        it will return true. Otherwise, if the queue is full, 
                        it will return false.

 Parameters     :       The new item to add to the list.
 
 Return         :       whether or not is was successful. 
 
 -----------------------------------------------------------------------------*/
    bool Push(T NewItem)
    {
        bool Success ;
        
        if (TheQueue.size() > MaxSize)
        {
            //We have a limit on the side of our queue, so if we have reached
            //that we do not want to push the new item, and we want to return false
            //so that the calling thread knows we have failed. 
            Success = false ; 
        }
        else
        {
            //We are fine for space! Let's add the new item and return that we are
            //happy.
            std::unique_lock<std::mutex> lock(QueueMutex) ;
            
            TheQueue.push(NewItem) ; 
            
            lock.unlock()  ;
            Success = true ;
        }
        
        return Success ; 
    }
    
    

 /*------------------------------------------------------------------------------
 
 Function Name  :       Pop
 
 Description    :       This function will check if the stack is empty, and
                        if it's not, it will get the item at the front of the 
                        queue and return it. If it is empty, it will wait for
                        the next item to arrive and then return that.

 Parameters     :       None
 
 Return         :       The first item in the queue or a zero value.
 
 -----------------------------------------------------------------------------*/
    T Pop(void)
    {
        T NewItem ;
        
        while (TheQueue.empty() == true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));  
        }
        //OK we have left that loop so we can lock the queue and return the value.
        std::unique_lock<std::mutex> lock(QueueMutex) ;
        
        NewItem = TheQueue.front() ; 
        
        TheQueue.pop() ; 
        lock.unlock()  ;
                
        return NewItem ; 
    }
    
    
    
 /*------------------------------------------------------------------------------
 
 Function Name  :       Pop_Try
 
 Description    :       This function will check if the stack is empty, and
                        if it's not, it will get the item at the front of the 
                        queue and return it. If it is empty, it will return 0.

 Parameters     :       None
 
 Return         :       The first item in the queue or a zero value.
 
 -----------------------------------------------------------------------------*/
    T Pop_Try(bool* Result)
    {
        T NewItem ; 
        
        if (TheQueue.empty())
        {
            //We have an empty list and therefore we can't
            //return anything.  Let's just return 0.
            *Result = false ; 
        }
        else
        {
            //We do have a list! Let's lock the queue so we can safely
            //manipulate it, and return the first item.
            std::unique_lock<std::mutex> lock(QueueMutex) ;
            
            NewItem = TheQueue.front() ; 
            
            TheQueue.pop() ; 
            lock.unlock()  ;
            
            *Result = true ; 
        }
        
        return NewItem ; 
    }
    

};

#endif /* MYQUEUE_H */

