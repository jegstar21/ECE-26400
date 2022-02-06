#include <stdio.h>
#include <stdlib.h>

#include "genome.h"

void FreeHashGraph(GNode** hashGraph, int GRAPH_SIZE){
  ///loop through your hashGraph and free all existing nodes
  ///Then free the hashGraph

  int l;

  for(l = 0; l < GRAPH_SIZE; l++);
  {
    GNode* helper = hashGraph[l];

    if(helper != NULL)
    {
      free(helper->key);
      free(helper);
    }
  }

  free(hashGraph);
  
}

void BuildGraph(char* inputFilename, char* nodeFilename, int k_len, GNode** hashGraph, int GRAPH_SIZE){
  int lineLen = getLineLength(inputFilename);
  FILE* ifptr = fopen(inputFilename, "r");
  if(!ifptr){
    printf("Could not open input file!!\n");
    FreeHashGraph(hashGraph, GRAPH_SIZE);
  }
  char* buffer = (char*)malloc(sizeof(char)*lineLen + 2); 
  GNode* prev=NULL, *curr=NULL;

  while(fgets(buffer, lineLen + 1, ifptr) != NULL)
  {
    int l;

    char* string = (char*) buffer;
    prev = NULL; 
    curr = NULL;

    for (l = 0; l < lineLen - k_len + 1; l++)
    {
      int hnode;

      hnode = hash(string, k_len, GRAPH_SIZE);

      if(hashGraph[hnode] == NULL)
      {
        GNode* new = (GNode*)malloc(sizeof(GNode));

        if(new == NULL)
        {
          printf("Malloc Error. Exiting program\n");
          FreeHashGraph(hashGraph, GRAPH_SIZE);
          exit(1);
        }

        InitGNode(new, string, k_len);
        hashGraph[hnode] = new;
      }

      curr = hashGraph[hnode];
      if(prev != NULL)
      {
        AddEdges(prev, curr);
        prev = curr;
        string++;
      }

      fgetc(ifptr);
    }
  }
  // for each line in the input file:
  //   prev=NULL;
  //   curr=NULL;
  //   for each k_len substring in the line:
  //     int nodeHash = hash(str, k_len, GRAPH_SIZE);///hash defined in genome.h
  //     check if node exists already(handle collisions). 
  //     If it doesnt exist, malloc and use InitGNode
  //     Code may be something like:
  //       GNode* newNode = (GNode*)malloc(sizeof(GNode));
  //       InitGNode(newNode, str, k_len);
  //     If there is an error at any point, make sure we free what we have malloc
  //       maybe use FreeHashGraph(hashGraph, GRAPH_SIZE); for ease
  //     set curr equal to the node, either the one we malloc or the one existing
  //     if(prev)
  //       AddEdges(prev, curr);///Definition in genome.h
  //     prev=curr;
}

void TraverseHelper(GNode* node, FILE* outfile){
  fprintf(outfile, "%c", node->key[node->key_len-1]);
  if(InDegree(node) == 1 && OutDegree(node) == 1){
    for(int j = 0; j < 4; ++j){
      if (node->outEdges[j]){
        TraverseHelper(node->outEdges[j], outfile);
        break;
      }
    }
  }
}

void TraverseGraph(GNode** hashGraph, int k_len, int GRAPH_SIZE, char* outputFilename){
  FILE* outfile = fopen(outputFilename, "w");

  int l;

  for(l = 0; l < GRAPH_SIZE; l++)
    {
      GNode* new = hashGraph[l];
      if (new && (InDegree(new) != 1 || OutDegree(new) > 1))
      {
        int k;
        for(k = 0; k < 4; k++)
        {
          if (new-> outEdges[k])
          {
            fprintf(outfile, "%s", new->key);
            TraverseHelper(new->outEdges[k], outfile);
            fprintf(outfile, "\n");
          }
        }
      }
    }
  /*Do some sort of loop that will start a traversal at every hnode
  * use TraverseHelper to traverse after we are at hnode to make this easier
  */
  fclose(outfile);
}

void PrintUniqueNodes(char* nodeFilename, GNode** hashGraph, int GRAPH_SIZE){
  FILE* nodeOut = fopen(nodeFilename, "w");
  /*do some sort of loop to print all unique nodes here*/

  int l;
  for (l = 0; l < GRAPH_SIZE; l++)
  {
    GNode* nodeNew = hashGraph[l];

    if (nodeNew != NULL)
    {
      fprintf(nodeOut, "%s\n", nodeNew->key);
    }
  }
  fclose(nodeOut);
}

int main(int argc, char* argv[]){
  // argv[1] should be an integer of what our k length should be.
  // argv[2] will be the input filename that will contain the different reads. Each line in the
  // input file corresponds to one read.
  // argv[3] will be the filename of where you should output the strings from your traversal
  if(argc != 5){
    printf("Usage: ./pa3 <k_len> <inputFilename> <outputFilename> <nodeoutputFilename>\n");
    exit(1);
  }
  int k_len = atoi(argv[1]);
  if(k_len < 1){
    printf("k_len must be an integer greater than 0\n");
    exit(1);
  }
  char* inputFilename = argv[2];
  char* outputFilename = argv[3];
  char* nodeFilename = argv[4];

  int GRAPH_SIZE = setGraphSize(k_len);///defined in genome.h
  GNode** hashGraph = (GNode**)malloc(sizeof(GNode*)*GRAPH_SIZE);
  for(int i = 0; i < GRAPH_SIZE; ++i){
    hashGraph[i] = NULL;//intialize just in case
  }

  ///Build the graph from the input file
  BuildGraph(inputFilename, nodeFilename, k_len, hashGraph, GRAPH_SIZE);
  
  ///Output the unique nodes
  PrintUniqueNodes(nodeFilename, hashGraph, GRAPH_SIZE);

  ///output of the output strings
  TraverseGraph(hashGraph, k_len, GRAPH_SIZE, outputFilename);

  ///Make sure we free the hashGraph correctly. 
  FreeHashGraph(hashGraph, GRAPH_SIZE);
  return 0;
}