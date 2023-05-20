#ifndef __FIFO_BUF_H__
#define __FIFO_BUF_H__

struct fifo_buf {
    unsigned char *buf;
    unsigned int head, tail;
};

/*返回该缓冲区head-tail之间的差值*/
#define FIFO_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/*刨去head-tail之间的大小，整个缓冲区还有多少空间*/
#define FIFO_SPACE(head,tail,size) FIFO_CNT((tail),((head)+1),(size))

/*head到整个缓冲区的结束位置被tail分成了两部分，取这两部分的最小值*/
#define FIFO_CNT_TO_END(head,tail,size) \
	({int end = (size) - (tail); \
	  int n = ((head) + end) & ((size)-1); \
	  n < end ? n : end;})

/*返回head到整个缓冲区结尾有多少空间可以用*/
#define FIFO_SPACE_TO_END(head,tail,size) \
	({int end = (size) - 1 - (head); \
	  int n = (end + (tail)) & ((size)-1); \
	  n <= end ? n : end+1;})

#endif
