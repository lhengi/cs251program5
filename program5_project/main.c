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
    if(pTemp->basic != 1)
    {
        fprintf(stderr, "***Error Can't touch basic file!\n");
        return;
    }
    if(pTemp == NULL)
    {
        fprintf(stderr, "***Error File does not exist\n");
        return;
    }
    
    pTemp->localTime = graph->clock;
    updateClock(graph);
    printf("%s Touched at: %d\n",filename,pTemp->localTime);
}

void timestamp(Graph* graph, char* filename)
{
    Node* pTemp = hmap_get(graph->map, filename);
    if(filename == NULL)
    {
        fprintf(stderr, "*** Error File does not exist\n");
        return;
    }
    printf("Last time modfiy %s, is at %d\n",filename,pTemp->localTime);
}

void allTimestamps(Graph* graph)
{
    
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
