//
//  main.c
//  program5_project
//
//  Created by Heng Li on 12/07/17.
//  Copyright © 2017 Heng Li. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"

typedef struct Node
{
    int localTime;
    int basic;
    int active;
    char* filename;
    struct Node* dependOn;
    
    
}Node;


typedef struct Graph
{
    int total_vertex;
    int clock;
    Node** adjacency;
    HMAP* map;
}Graph;


Node* createNode(char* file, int basic)
{
    if(file == NULL || file[0] == '\0')
    {
        return NULL;
    }
    Node* newNode;
    newNode = malloc(sizeof(Node));
    
    newNode->basic = basic;
    newNode->active = 0;
    newNode->filename = malloc(sizeof(char)*256);
    strcpy(newNode->filename, file);
    newNode->dependOn = NULL;
    newNode->localTime = 0;
    return newNode;
}

void createNodeLink(Node* vertex, char* edge)
{
    Node* ed = createNode(edge,0);
    ed->dependOn = vertex->dependOn;
    vertex->dependOn = ed;
}

// return 0 if the format is wrong
int readLine(Graph* graph, char* line)
{
    //printf("Line: %s\t",line);
    char* tok;
    char* tok2; // this is the variable to handle the colon
    tok = strtok(line, " \n\r");//added carrige return just to be safe
    if(tok == NULL || tok[0] == '\0' )// prevent hash function get a NULL
        return 1;
    if(strcmp(tok, ":") == 0)
    {
        fprintf(stderr, "Wrong Format!\n");
        return 0;
    }
    
    if(hmap_get(graph->map, tok) != NULL)
    {
        fprintf(stderr, "***Error Duplicate File\n");
        return 0;
    }
    
    //printf("%s\n",tok);
    tok2 = strtok(NULL, " \n\r");
    char* filename = tok;
    Node* basic;
    Node* basic2;
    if(tok2 == NULL)
    {
        basic = createNode(filename,1);
        hmap_set(graph->map, tok, basic);
        graph->total_vertex++;
        //basic2 =  hmap_get(graph->map, filename);
        return 1;
    }
    
    if(strcmp(tok2, ":") != 0)
    {
        fprintf(stderr, "Wrong Format!\n");
        return 0;
    }
    
    // reuse tok2 to make sure other side of the colon is not empty
    tok2 = strtok(NULL, " \n\r");
    if(tok2 == NULL)
    {
        fprintf(stderr, "Wrong Format!\n");
        return 0;
    }
    
    
    Node* pTemp = createNode(tok,0);
    createNodeLink(pTemp, tok2);
    
    while (tok != NULL)
    {
        tok = strtok(NULL, " \n\r");
        if(tok != NULL && strcmp(tok, ":") == 0)
        {
            fprintf(stderr, "Wrong format!\n");
            return 0;
        }
        if(tok != NULL && tok[0] != '\0')
        {
            createNodeLink(pTemp, tok);
        }
    }
    hmap_set(graph->map, pTemp->filename, pTemp);
    graph->total_vertex++;
    
    return 1;
}

Graph* createGraph()
{
    Graph* graph;
    graph = malloc(sizeof(Graph));
    
    graph->clock = 0;
    graph->adjacency = NULL;
    graph->total_vertex = 0;
    graph->map = hmap_create(1024, 0.75);
    return graph;
}

void updateClock(Graph* graph)
{
    graph->clock++;
}

int getTime(Graph* graph)
{
    return graph->clock;
}

void touch(Graph* graph, char* filename)
{
    Node* pTemp = hmap_get(graph->map, filename);
    if(pTemp == NULL)
    {
        fprintf(stderr, "***Error File does not exist\n");
        return;
    }
    if(pTemp->basic != 1)
    {
        fprintf(stderr, "***Error Can't touch None basic file!\n");
        return;
    }
    
    updateClock(graph);
    pTemp->localTime = graph->clock;
    
    printf("%s Touched at: %d\n",filename,pTemp->localTime);
}

void timestamp(Graph* graph, char* filename)
{
    Node* pTemp = hmap_get(graph->map, filename);
    if(filename == NULL || pTemp == NULL)
    {
        fprintf(stderr, "*** Error File does not exist\n");
        return;
    }
    printf("Last time modfiy %s, is at %d\n",filename,pTemp->localTime);
}

void allTimestamps(Graph* graph)
{
    int n = graph->total_vertex;
    int i;
    Node* pTemp;
    for(i = 0; i < n; i++)
    {
        pTemp = graph->adjacency[i];
        if(pTemp == NULL)
            return;
        printf("File Name: %s\tTimeStamp: %d\n",pTemp->filename,pTemp->localTime);
        
    }
}

int validateVertex(Graph* graph, char* filename)
{
    Node* vertex = hmap_get(graph->map, filename);
    if(vertex == NULL)
    {
        fprintf(stderr, "Filename: %s, Does not exist\n",filename);
        return 0;
    }
    if(vertex->active == 1 && vertex->basic == 0)
    {
        fprintf(stderr, "There's a Cycle\n");
        return 0;
    }
    vertex->active = 1;
    
    
    // this is a basic file
    if(vertex->basic == 1)
    {
        return 1;
    }
    
    Node* pTemp;
    pTemp = vertex->dependOn;
    Node* pTemp_inGraph;
    while (pTemp != NULL)
    {
        pTemp_inGraph = hmap_get(graph->map, pTemp->filename);
        //pTemp_inGraph->active = 1;
        if(validateVertex(graph, pTemp->filename) == 0)
        {
            return 0;
        }
        //pTemp_inGraph->active = 0;
        
        pTemp = pTemp->dependOn;
    }
    
    vertex->active = 0;
    return 1;
}

int validateGraph(Graph* graph)
{
    int n = graph->total_vertex;
    int i;
    Node* pTemp;
    for(i = 0; i < n; i++)
    {
        pTemp = graph->adjacency[i];
        if(pTemp == NULL)
            return 1;
        if(validateVertex(graph, pTemp->filename) == 0)
        {
            fprintf(stderr, "Something wrong with this file %s\n",pTemp->filename);
            return 0;
        }
    }
    
    return 1;
}

// this will return time stamp
int makeFile(Graph* graph, char* filename)
{
    Node* vertex = hmap_get(graph->map, filename);
    if(vertex == NULL)
    {
        fprintf(stderr, "****Error File does not exist\n");
        return -1;
    }
    if(vertex->basic == 1)
    {
        return vertex->localTime;
    }
    
    Node* pTemp;
    Node* pTemp_inGraph;
    
    pTemp = vertex->dependOn;
    
    int depend_uptoDate = 1;
    while(pTemp != NULL)
    {
        pTemp_inGraph = hmap_get(graph->map, pTemp->filename);
        
        if(makeFile(graph, pTemp_inGraph->filename) >= vertex->localTime)
        {
            depend_uptoDate = 0;
        }
        pTemp = pTemp->dependOn;
    }
    
    // doesn't need to update
    if(depend_uptoDate)
    {
        printf("%s is up to date\n",vertex->filename);
        return vertex->localTime;
    }
    
    printf("making %s... done\n",vertex->filename);
    updateClock(graph);
    vertex->localTime = graph->clock;
    
    return vertex->localTime;
    
}

void makeFileHelper(Graph* graph, char* filename)
{
    Node* pTemp = hmap_get(graph->map, filename);
    if(pTemp == NULL)
    {
        fprintf(stderr, "***Error File does not exist\n");
        return;
    }
    if(pTemp->basic == 1)
    {
        fprintf(stderr, "You can't make a Basic file\n");
        return;
    }
    printf("Making %s:\n",filename);
    makeFile(graph, filename);
}



int main(int argc, const char * argv[])
{
    // insert code here...
    printf("Hello, World!\n");
    Graph* graph = createGraph();
    char* filename;
    filename = argv[1];
    FILE* inputFile;
    inputFile = fopen("testInput1.txt", "r");
    char* line;
    int formatStatus = 1;
    while (!feof(inputFile))
    {
        line = malloc(sizeof(char)*256);
        
        fgets(line, 256, inputFile);

        formatStatus = readLine(graph, line);
        if (formatStatus == 0)
        {
            fprintf(stderr, "Wrong format\n");
            return 0;
        }
        free(line);
    }
    fclose(inputFile);
    
    //set the adjacency array
    graph->adjacency = hmap_extract_values(graph->map);
    
    if(validateGraph(graph) == 0)
    {
        fprintf(stderr, "***Error in the file, cycle or file does not exist \n");
        return 0;
    }
    
    char* userInput1;
    char* userInput2;
    userInput1 = malloc(sizeof(char)*256);
    userInput2 = malloc(sizeof(char)*256);
    printf("Select your input:\ntime\ntouch <filename>\ntimestamp <filename>\ntimestamps\nmake <target>\nquit\n");
    while (1)
    {
        printf("\nEnter Here:");
        scanf(" %s",userInput1);
        userInput1 = strtok(userInput1, " \n\r");
        
        printf("\n");
        
        if(strcmp(userInput1, "time") == 0)
        {
            printf("Global Clock is at: %d\n",getTime(graph));
        }
        else if(strcmp(userInput1, "timestamps") == 0)
        {
            allTimestamps(graph);
        }
        else if(strcmp(userInput1, "touch") ==0 )
        {
            scanf(" %s",userInput2);
            userInput2 = strtok(userInput2, " \n\r");
            if(userInput2 != NULL)
                touch(graph, userInput2);
        }
        else if (strcmp(userInput1, "timestamp") == 0 )
        {
            scanf(" %s",userInput2);
            if (userInput2 != NULL)
                timestamp(graph, userInput2);
        }
        else if (strcmp(userInput1, "make") == 0 )
        {
            scanf(" %s",userInput2);
            if ( userInput2 != NULL)
                makeFileHelper(graph, userInput2);
        }
        else if (strcmp(userInput1, "quit") == 0)
        {
            printf("Thanks for using. Exiting.....\n");
            return 0;
        }
        else
        {
            printf("Command not recognized\n");
        }
    }
    
    /*
    if(hmap_contains(graph->map, "E"))
        printf("Contains\n");
     
    Node* testNode = hmap_get(graph->map, "E");
    printf("TestNode, File name:%s\n",testNode->filename);
    printf("TestNode, local Time:%d\n",testNode->localTime);
    Node* pTemp;
    pTemp = testNode->dependOn;
    while (testNode->basic == 0 && pTemp != NULL)
    {
        printf("TestNode, Dependon: %s\n",pTemp->filename);
        pTemp = pTemp->dependOn;
    }
    */
    
    
    
    return 0;
}
