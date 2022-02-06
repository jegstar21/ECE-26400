#ifndef __GENOME_H_
#define __GENOME_H_

#include <string.h>
#include <limits.h>

#define MAX_KMER_LEN 16

int getval(char c){
  switch (c){
    case 'A': case 'a':
      return 0;
    case 'C': case 'c':
      return 1;
    case 'G': case 'g':
      return 2;
    case 'T': case 't':
      return 3;
    default:
      return 0;
  }
  return 0;
}

typedef struct GNode{
  char* key;/// Will hold strings only consisting of ACGT
  struct GNode* inEdges[4];
  struct GNode* outEdges[4];
  int key_len;/// this is our k length
}GNode;

void InitGNode(GNode* node, char* k, int kl){
  int max_chars = kl < MAX_KMER_LEN ? kl : MAX_KMER_LEN;
  if(max_chars != kl){
    printf("Can't save the length of the key correctly. Exiting program.\n");
    exit(1);
  }
  strncpy(node->key, k, kl);
  node->key[kl] = '\0';
  node->key_len = kl;
}

int hash(char* key, int key_len, int GRAPH_SIZE){///this can be used to determine if string exists already
  int toRet = 0;
  int i;
  for(i = 0; i < key_len; ++i){
    toRet = toRet << 2;
    toRet |= getval(key[i]);
    }
  toRet = toRet % GRAPH_SIZE;
  return toRet;
}

void AddEdges(GNode* from, GNode* to){
      from->outEdges[getval(to->key[to->key_len-1])] = to;
      to->inEdges[getval(from->key[0])] = from;
}

int setGraphSize(int k_len){
      int GRAPH_SIZE = 1;
      switch(k_len){
          case 1: GRAPH_SIZE = 4; break;
          case 2: GRAPH_SIZE = 16; break;
          case 3: GRAPH_SIZE = 64; break;
          case 4: GRAPH_SIZE = 256; break;
          case 5: GRAPH_SIZE = 1024; break;
          case 6: GRAPH_SIZE = 4096; break;
          case 7: GRAPH_SIZE = 16384; break;
          case 8: GRAPH_SIZE = 65536; break;
          case 9: GRAPH_SIZE = 262144; break;
          case 10: GRAPH_SIZE = 1048576; break;
          case 11: GRAPH_SIZE = 4194304; break;
          default: GRAPH_SIZE = 16777216; break;
      }
      return GRAPH_SIZE;
}

int InDegree(GNode* degNode)
{
  int edges = 0;
  int l;

  for(l = 0; l < 4; l++)
  {
    if (degNode->inEdges != NULL)
    {
      edges++;
    }
  }

  return edges;
}

int OutDegree(GNode* degNodeTwo)
{
  int edges = 0;
  int l;

  for(l = 0; l < 4; l++)
  {
    if (degNodeTwo->outEdges != NULL)
    {
      edges++;
    }
  }

  return edges;
}

int getLineLength(char* file)
{
  FILE* ptr = fopen(file, "r");
  if (ptr == NULL)
  {
    printf("Error opening file. Exiting with code FAILURE\n");
    return EXIT_FAILURE;
  }

  int a = fgetc(ptr);
  int length = 0;

  while((a != '\n' && a != EOF))
  {
    length++;
  }

  fclose(ptr);
  
  return length;
}

#endif //__GENOME_H_