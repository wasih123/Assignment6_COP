/*
 * cache.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cache_part1.h"
#include "main_part1.h"

/* cache configuration parameters */
static int cache_split = 0;
static int cache_usize = DEFAULT_CACHE_SIZE;
static int cache_isize = DEFAULT_CACHE_SIZE; 
static int cache_dsize = DEFAULT_CACHE_SIZE;
static int cache_block_size = DEFAULT_CACHE_BLOCK_SIZE;
static int words_per_block = DEFAULT_CACHE_BLOCK_SIZE / WORD_SIZE;
static int cache_assoc = DEFAULT_CACHE_ASSOC;
static int cache_writeback = DEFAULT_CACHE_WRITEBACK;
static int cache_writealloc = DEFAULT_CACHE_WRITEALLOC;
int cunt =0;
/* cache model data structures */
static Pcache icache;
static Pcache dcache;
static cache c1;
static cache c2;
static cache c;
static cache_stat cache_stat_inst;
static cache_stat cache_stat_data;
static int counter=0;

/************************************************************/
void set_cache_param(param, value)
  int param;
  int value;
{

  switch (param) {
  case CACHE_PARAM_BLOCK_SIZE:
    cache_block_size = value;
    words_per_block = value / WORD_SIZE;
    break;
  case CACHE_PARAM_USIZE:
    cache_split = FALSE;
    cache_usize = value;
    break;
  case CACHE_PARAM_ISIZE:
    cache_split = TRUE;
    cache_isize = value;
    break;
  case CACHE_PARAM_DSIZE:
    cache_split = TRUE;
    cache_dsize = value;
    break;
  case CACHE_PARAM_ASSOC:
    cache_assoc = value;
    break;
  case CACHE_PARAM_WRITEBACK:
    cache_writeback = TRUE;
    break;
  case CACHE_PARAM_WRITETHROUGH:
    cache_writeback = FALSE;
    break;
  case CACHE_PARAM_WRITEALLOC:
    cache_writealloc = TRUE;
    break;
  case CACHE_PARAM_NOWRITEALLOC:
    cache_writealloc = FALSE;
    break;
  default:
    printf("error set_cache_param: bad parameter value\n");
    exit(-1);
  }

}
/************************************************************/

/************************************************************/

Pcache_line access_cache(int i,Pcache c)
{ 
	/*
	********Function to return a cache_line for passed cache*********
	*/

	Pcache_line temp;
	temp = c->LRU_head;
	int j;

	for(j=0;j<i-1;j++)
	{
	temp=temp->LRU_next;
	}

	if(i!=0)
	temp=temp->LRU_next;

	return temp;
}

void initialize_cache(Pcache c){
	/*
	****Modular unit which allocates space to a given passed cache*******
	*/

	int i,c_lines;	
	/*
	if(i_or_d == 0)
		c_lines = ((c->n_sets);
	else if (i_or_d == 1)
		c_lines = (cache_isize)/(c->n_sets);
	else
		c_lines = (cache_dsize)/(c->n_sets);
	*/
	c_lines = (c -> n_sets);
	

	Pcache_line temp;
	temp= malloc(sizeof(cache_line));
	temp->word1 = 0;
	temp->word2 = 0;
	temp->word3 = 0;
	temp->word4 = 0;
	temp->dirty = 0;
	temp->tag = -1;
	temp->LRU_count=0;
	temp->LRU_next=NULL;
	temp->LRU_prev=NULL;
	Pcache_line Temp=temp;	

	for(i=1; i<(c->associativity); i++){
		Pcache_line temp1;
		temp1= malloc(sizeof(cache_line));
		temp1->word1 = 0;
		temp1->word2 = 0;
		temp1->word3 = 0;
		temp1->word4 = 0;
		temp1->dirty = 0;
		temp1->tag = -1;
		temp->LRU_count=0;
		temp1->LRU_next=NULL;
		temp1->LRU_prev=NULL;
		Temp->LRU_adj=temp1;	
		Temp=temp1;
	}
	Temp->LRU_adj=NULL;
		
	c->LRU_head = temp;

	temp= malloc(sizeof(cache_line));
	temp->word1 = 0;
	temp->word2 = 0;
	temp->word3 = 0;
	temp->word4 = 0;
	temp->dirty = 0;
	temp->tag = -1;
	temp->LRU_count=0;
	temp->LRU_next=NULL;
	temp->LRU_prev=NULL;
	Pcache_line TTemp=temp;	

	for(i=1; i<(c->associativity); i++){
		Pcache_line temp1;
		temp1= malloc(sizeof(cache_line));
		temp1->word1 = 0;
		temp1->word2 = 0;
		temp1->word3 = 0;
		temp1->word4 = 0;
		temp1->dirty = 0;
		temp1->tag = -1;
		temp->LRU_count=0;
		temp1->LRU_next=NULL;
		temp1->LRU_prev=NULL;	
		TTemp->LRU_adj=temp1;	
		TTemp=temp1;
	}
	TTemp->LRU_adj=NULL;
	
	c->LRU_tail = temp;
	c->LRU_tail->LRU_prev = c->LRU_head;

	c->LRU_head->LRU_next = c->LRU_tail;
	 
 	for( i=1; i<c_lines-1 ;i++)
	{
		temp= malloc(sizeof(cache_line));
		temp->word1 = 0;
		temp->word2 = 0;
		temp->word3 = 0;
		temp->word4 = 0;
		temp->dirty = 0;
		temp->tag = -1;
		temp->LRU_count=0;
		temp->LRU_next=NULL;
		temp->LRU_prev=c->LRU_tail;
		Pcache_line TTTemp=temp;	
		int j;
		
		for(j=1; j<(c->associativity); j++){
			Pcache_line temp1;
			temp1= malloc(sizeof(cache_line));
			temp1->word1 = 0;
			temp1->word2 = 0;
			temp1->word3 = 0;
			temp1->word4 = 0;
			temp1->dirty = 0;
			temp1->tag = -1;
			temp->LRU_count=0;
			temp1->LRU_next=NULL;
			temp1->LRU_prev=NULL;	
			TTTemp->LRU_adj=temp1;
			TTTemp=temp1;
		}
		TTTemp->LRU_adj=NULL;
		c->LRU_tail->LRU_next = temp;
		c->LRU_tail = temp;
	}
}
/************************************************************/

/************************************************************/

void init_cache()
{

  /* initialize the cache, and cache statistics data structures */
  
	  c1.size=cache_isize;
	  c2.size=cache_dsize;
	  c.size=cache_usize;
	  c1.associativity=cache_assoc;
	  c2.associativity=cache_assoc; 
	  c.associativity=cache_assoc;  
	  c1.n_sets=cache_isize/(cache_assoc*cache_block_size);
	  c2.n_sets=cache_dsize/(cache_assoc*cache_block_size);
	  c.n_sets=cache_usize/(cache_assoc*cache_block_size);	

	  initialize_cache(&c1);
	  initialize_cache(&c2);
	  initialize_cache(&c);
 
	  cache_stat_inst.accesses=0;
	  cache_stat_inst.misses=0;
	  cache_stat_inst.replacements=0;
	  cache_stat_inst.demand_fetches=0;
	  cache_stat_inst.copies_back=0;

	  cache_stat_data.accesses=0;
	  cache_stat_data.misses=0;
	  cache_stat_data.replacements=0;
	  cache_stat_data.demand_fetches=0;
	  cache_stat_data.copies_back=0;
}
/************************************************************/

/************************************************************/

int contains(int tag,int at,Pcache_line line){
	Pcache_line temp=line;
	
	int i;
	for(i=0;i<cache_assoc-1;i++){
		if(tag == temp->tag){
			temp->LRU_count=counter;
			if(at == 1)
				temp->dirty = 1;
			return 1;
		}
		temp=temp->LRU_adj;
	}
	
	if(temp->tag == tag){
		temp->LRU_count=counter;
		if(at == 1)
			temp->dirty = 1;
		return 1;
	}
	
	return 0;	//tag not matched
	
}

int wasplace(int tag,int at,Pcache_line line){
	Pcache_line temp = line;
	int i;
	for(i=0;i<cache_assoc-1;i++){
		if(temp->tag == -1){
		temp->tag = tag;
		temp->LRU_count=counter;
		if(at == 1)
			temp->dirty = 1;
		return 1;	
		}
		temp=temp->LRU_adj;
	}
	
	if(temp->tag == -1){
		temp->tag = tag;
		temp->LRU_count=counter;
		if(at == 1)
			temp->dirty = 1;
		return 1;
	}
	
	return 0;
}

Pcache_line replace(int tag,int at,Pcache_line line){
	Pcache_line temp = line;
	Pcache_line temp1 = line;
	int m_count = temp->LRU_count;
	
	int i;
	for(i=0;i<cache_assoc-1;i++){
		temp = temp->LRU_adj;
		if(m_count>temp->LRU_count){
			m_count=temp->LRU_count;
			temp1 = temp;
		}
	}
	
	if(temp1->dirty == 1){
		if(at == 2)
		cache_stat_inst.copies_back+=cache_block_size/4;
		else
		cache_stat_data.copies_back+=cache_block_size/4;
	}
		
	temp1->tag=tag;
	temp1->LRU_count = counter;
	if(temp1->dirty == 0 && at == 1)
		temp1->dirty = 1;
	return temp1; 
}


void perform_access(addr, access_type)
  unsigned addr, access_type;
{
int index;
int Tag;  
counter++;
   
  /* handle an access to the cache */
	
	if(access_type==0){
		Pcache_line line;
		cache_stat_data.accesses++;
		if(cache_split == 1){  //i.e. cache is not unified
			index = (int)(addr/(cache_block_size))%(cache_dsize/(cache_block_size*cache_assoc));
			line = access_cache(index,&c1);
			Tag=(addr*cache_assoc/cache_dsize);
		}
		else{
			index = (int)(addr/(cache_block_size))%(cache_usize/(cache_block_size*cache_assoc));
			line = access_cache(index,&c);
			Tag=(addr*cache_assoc/cache_usize);	
		}	
		int tag=line->tag;
		int vbit=line->dirty;
		
		if(contains(Tag,0,line) != 1){
			cache_stat_data.misses++;
			cache_stat_data.demand_fetches+=cache_block_size/4;
			if(wasplace(Tag,0,line) != 1){
				cache_stat_data.replacements++;
				Pcache_line l = replace(Tag,0,line);
				l->dirty=0;
			}
		}
	}
		
	//******load done*******

	else if(access_type==1){ 
		cache_stat_data.accesses++;
		Pcache_line line;
		if(cache_split == 1){  //i.e. cache is not unified	
			index = (int)(addr/(cache_block_size))%(cache_dsize/(cache_block_size*cache_assoc));
			line = access_cache(index,&c1);
			Tag=(addr*cache_assoc/cache_dsize);
		}
		else{
			index = (int)(addr/(cache_block_size))%(cache_usize/(cache_block_size*cache_assoc));
		    line = access_cache(index,&c);
		    Tag=(addr*cache_assoc/cache_usize);
		}

		int tag=line->tag;
		int vbit=line->dirty;
		Pcache_line l;
		
		if(contains(Tag,1,line) != 1){
			cache_stat_data.misses++;
			if(cache_writealloc==0)
				cache_stat_data.copies_back++;
			else{
				cache_stat_data.demand_fetches+=cache_block_size/4;
				if(wasplace(Tag,1,line) != 1){
					cache_stat_data.replacements++;
					l = replace(Tag,1,line);
				}
			}
		}
		//if(l->dirty==0) l->dirty=1;		
	}
	
	//*******store done********
			
	else if(access_type==2){
		cache_stat_inst.accesses++;
		Pcache_line line;
		if(cache_split == 1){  //i.e. cache is not unified
			   index = (int)(addr/(cache_block_size))%(cache_isize/(cache_block_size*cache_assoc));
		       line = access_cache(index,&c2);
		       Tag=(addr*cache_assoc/cache_isize);
		   }
		else{  
			   index = (int)(addr/(cache_block_size))%(cache_usize/(cache_block_size*cache_assoc));
		       line = access_cache(index,&c);
		       Tag=(addr*cache_assoc/cache_usize);
		   }

		int tag=line->tag;
		int vbit=line->dirty;
			
		if(contains(Tag,2,line) != 1){
			cache_stat_inst.misses++;
			
			cache_stat_inst.demand_fetches+=cache_block_size/4;
			if(wasplace(Tag,2,line) != 1){
				cache_stat_inst.replacements++;
				Pcache_line l = replace(Tag,2,line);
				l->dirty=0;
			}
		}
	}
		
	//********instruction done*********
}

/************************************************************/

/************************************************************/

void checkdirty(Pcache_line line,int i_or_d){
int i;
Pcache_line temp=line;
for(i=0;i<cache_assoc-1;i++){
	if(temp->dirty==1){
		if(i_or_d == 0)	//instruction
			cache_stat_inst.copies_back+=cache_block_size/4;
		else if(i_or_d == 1)	//data
			cache_stat_data.copies_back+=cache_block_size/4;
		temp->dirty=0;
	}
	temp=temp->LRU_adj;
}
if(temp->dirty==1){
		if(i_or_d == 0)	//instruction
			cache_stat_inst.copies_back+=cache_block_size/4;
		else if(i_or_d == 1)	//data
			cache_stat_data.copies_back+=cache_block_size/4;
		temp->dirty=0;
	}
}

void flush()
{
			
  /* flush the cache */

	if(cache_split == 1){

		Pcache_line p = (c1.LRU_head);
		int i;
		int vbit;
		for(i=0;i<(c1.size/(cache_block_size*cache_assoc)-1);i++){
			vbit=p->dirty;
			checkdirty(p,1);
			p=p->LRU_next;
		}
			checkdirty(p,1);
		
		//data cache flushed
	
		p = (c2.LRU_head);
		for(i=0;i<(c2.size/(cache_block_size*cache_assoc)-1);i++){
			vbit=p->dirty;
			checkdirty(p,0);
			p=p->LRU_next;
		}
			checkdirty(p,0);
			
		//inst cache flushed
	}

	else{
		Pcache_line p = (c.LRU_head);
		int i;
		int vbit;
		for(i=0;i<(c.size/(cache_block_size*cache_assoc)-1);i++){
			vbit=p->dirty;
			checkdirty(p,1);
			p=p->LRU_next;
		}
			checkdirty(p,1);
	}
}

/************************************************************/

/************************************************************/

void delete(head, tail, item)
  Pcache_line *head, *tail;
  Pcache_line item;
{
  if (item->LRU_prev) {
    item->LRU_prev->LRU_next = item->LRU_next;
  } else {
    /* item at head */
    *head = item->LRU_next;
  }

  if (item->LRU_next) {
    item->LRU_next->LRU_prev = item->LRU_prev;
  } else {
    /* item at tail */
    *tail = item->LRU_prev;
  }
}

/************************************************************/

/************************************************************/
/* inserts at the head of the list */

void insert(head, tail, item)
  Pcache_line *head, *tail;
  Pcache_line item;
{
  item->LRU_next = *head;
  item->LRU_prev = (Pcache_line)NULL;

  if (item->LRU_next)
    item->LRU_next->LRU_prev = item;
  else
    *tail = item;

  *head = item;
}
/************************************************************/

/************************************************************/
void dump_settings()
{
  printf("*** CACHE SETTINGS ***\n");
  if (cache_split) {
    printf("  Split I- D-cache\n");
    printf("  I-cache size: \t%d\n", cache_isize);
    printf("  D-cache size: \t%d\n", cache_dsize);
  } else {
    printf("  Unified I- D-cache\n");
    printf("  Size: \t%d\n", cache_usize);
  }
  printf("  Associativity: \t%d\n", cache_assoc);
  printf("  Block size: \t%d\n", cache_block_size);
  printf("  Write policy: \t%s\n", 
	 cache_writeback ? "WRITE BACK" : "WRITE THROUGH");
  printf("  Allocation policy: \t%s\n",
	 cache_writealloc ? "WRITE ALLOCATE" : "WRITE NO ALLOCATE");
}
/************************************************************/

/************************************************************/
void print_stats()
{
  printf("\n*** CACHE STATISTICS ***\n");
  	
  printf(" INSTRUCTIONS\n");
  printf("  accesses:  %d\n", cache_stat_inst.accesses);
  printf("  misses:    %d\n", cache_stat_inst.misses);
  if (!cache_stat_inst.accesses)
    printf("  miss rate: 0 (0)\n"); 
  else
    printf("  miss rate: %2.4f (hit rate %2.4f)\n", 
	 (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses,
	 1.0 - (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses);
  printf("  replace:   %d\n", cache_stat_inst.replacements);

  printf(" DATA\n");
  printf("  accesses:  %d\n", cache_stat_data.accesses);
  printf("  misses:    %d\n", cache_stat_data.misses);
  if (!cache_stat_data.accesses)
    printf("  miss rate: 0 (0)\n"); 
  else
    printf("  miss rate: %2.4f (hit rate %2.4f)\n", 
	 (float)cache_stat_data.misses / (float)cache_stat_data.accesses,
	 1.0 - (float)cache_stat_data.misses / (float)cache_stat_data.accesses);
  printf("  replace:   %d\n", cache_stat_data.replacements);

  printf(" TRAFFIC (in words)\n");
  printf("  demand fetch:  %d\n", cache_stat_inst.demand_fetches + 
	 cache_stat_data.demand_fetches);
  printf("  copies back:   %d\n", cache_stat_inst.copies_back +
	 cache_stat_data.copies_back);
}
/************************************************************/

