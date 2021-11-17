#include<stdio.h>
#include<String.h>
#include<stdlib.h> 
#define true 1
#define false 0
#define maxsize 1000
#define io_size 20

char member[20];

typedef struct{
	char data[maxsize]; //存放字符串 
	int length; //存放串长
}SqString;

typedef struct Tree{ 
	char name[io_size];           
	char sex[2];              
	char fed[io_size];//配偶
	int level;//辈分
	struct Tree *child;              //孩子指针
	struct Tree *brother;            //兄弟指针
	struct Tree *parent;             //父亲指针
}Tree;

typedef struct hashtable{
	char name[io_size];
	Tree * tree; 
	struct hashtable* next; 
}hashtable; 

	hashtable* alphabet[26]={NULL};//做全局就怕在不相关的函数里修改了（如果传入的形参名字一样倒没什么）

void for_print(Tree* base,int status);

int locate_hash(char s[]){//根据最末字符定位哈希表
	int i=io_size-2;
	for(i=0;s[i]!='\0';i++);i--;	
	if(s[i]>='A'&&s[i]<='Z')return (s[i]-'A')%26;
	if(s[i]>='a'&&s[i]<='z')return (s[i]-'a')%26;
	else return s[i]%26;
}

void insert_hash(Tree* pedigree){//插入哈希表
	int p; hashtable* np;
	p=locate_hash(pedigree->name);
	np=(hashtable*)malloc(sizeof(hashtable));
	if(NULL==np){
		printf("内存不足\n");
		return ;}
	strcpy(np->name,pedigree->name);
	np->next=alphabet[p];
	np->tree=pedigree;
	alphabet[p]=np;	
}

hashtable* search_hash(char s[]){//在哈希表中查找，主要是为了应对重名的人
	hashtable* arrays[maxsize]={NULL};
	int p=locate_hash(s),i=-1,j=0;
	hashtable* np;

	for(np=alphabet[p];np!=NULL;np=np->next)
		if(strcmp(np->name,s)==0){arrays[++i]=np;}
	if(i==-1)return NULL;
	if(i>=1){
		printf("按下键盘选择编号\n");
		for(j=0;j<=i;j++){
			printf("编号%d---",j);for_print(arrays[j]->tree,1);
		}
		scanf("%d",&j);getchar();
		//printf("u chose NO.%d\n",j);		
	}
	return arrays[j];
}

void delete_hash(Tree* del){//在哈希表中删除
	if(del==NULL)return;
	hashtable* delete=alphabet[locate_hash(del->name)],*temp;
	for(;delete->tree!=del;delete=delete->next);							
	if(delete==NULL);
	else{				
		if(delete->next!=NULL){
			strcpy(delete->name,delete->next->name);
			delete->tree=delete->next->tree;
			temp=delete->next;
			delete->next=delete->next->next;
			//free(temp->name);//temp->name=NULL;
			free(temp);
			temp=NULL;
		}
		else{
			if(alphabet[locate_hash(del->name)]==delete){
				free(alphabet[locate_hash(del->name)]);
				alphabet[locate_hash(del->name)]=NULL;
			}
			else{ 
				hashtable* np;
				for(np=alphabet[locate_hash(del->name)];np->next!=delete;np=np->next);
				np->next=NULL;
				//free(delete->name);delete->name=NULL;
				free(delete);
				delete=NULL;
			}
		}
	}	
}

void test_hash(){//测试哈希表正常否
	int i=0;hashtable* np;
	for (;i<26;i++){
		for(np=alphabet[i];np!=NULL;np=np->next)printf("alphabet[%d]=%s\n",i,np->name);	
	}
}

void for_print(Tree* base,int status){//打印成员信息的统一函数
	if(base==NULL){printf("吃饱了没事干？\n");return;}
	else{
		if(status==1){
			if(base->parent==NULL)printf("父亲不详---");
			else printf("父亲是%s---",base->parent->name);
		}
		printf("辈分:第%d代---",base->level);
		printf("【名字】:%s---",base->name);
		if(base->sex[0]=='\0')printf("性别:男---");else printf("性别:女---");																			
     	if(base->fed[0]=='\0')printf("配偶:无或不详---\n");else printf("配偶:%s---\n",base->fed);
	}
}

int for_scan(Tree*base){//输入成员信息的统一函数
	char*to0;
	printf("姓名:");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->name,member);
	if('\0'==base->name[0])return 0;	
	printf("性别(直接回车默认男,其他随便输是女):");
	fgets(member,2,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->sex,member);
	printf("配偶名称(直接回车默认无，其他随便输是名字):");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n'); if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->fed,member);
	
	return 1;
}

Tree *find_member(char cname[]){//连接哈希表和树的桥梁

	hashtable*temp=search_hash(cname);
	if(temp==NULL)return NULL;
	return temp->tree;	
}

Tree* add_member(Tree *base){ //添加成员
	char option[2]={'\0','\0'};char* to0;
	printf("添加成员:");
	Tree *ptr,*to_target; 
	if(NULL==(ptr=(Tree*)malloc(sizeof(Tree)))){
		printf("内存不足\n");
		return NULL;}
	
	if(0==for_scan(ptr))return base;
	
	if(base==NULL){
	printf("这是初代目！\n");
	ptr->brother=NULL;                     
	ptr->child=NULL;     
	ptr->parent=NULL;
	ptr->level=1; 		//记录辈分，为第一代
	base=ptr;
	}
	else{
		printf("请输入要添加关系的目标成员的名称:");
		fgets(member,io_size,stdin);
		to0=NULL;to0=strchr(member,'\n');if(to0)*to0 = '\0';fflush(stdin);
		to_target=find_member(member);
		if(NULL==to_target){printf("目标不存在");return base;}	
		else {
			if(to_target!=base){
				printf("该节点不是祖宗，添加该节点的  1:兄弟, 其他键(包括回车):儿子    "); 
				fgets(option,2,stdin);
				to0=NULL;to0 = strchr(option, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			}	
			if(to_target!=base&&option[0]=='1'){//插入兄弟
				printf("添加的是目标的兄弟\n");
				for(;to_target->brother!=NULL;to_target=to_target->brother);
				ptr->level=to_target->level;   
				ptr->child=NULL;                
				ptr->brother=NULL;               
				ptr->parent=to_target->parent; 
				to_target->brother=ptr;
				insert_hash(ptr);
				return base;
			}
			printf("添加的是目标的儿子\n");
			ptr->level=to_target->level+1;              
			ptr->child=NULL;                  
			ptr->brother=NULL;                
			ptr->parent=to_target;                    //父亲结点就是此函数刚开始时的查找到的，然后进行赋值
			if(to_target->child==NULL)to_target->child=ptr;
			else{                         //如果有孩子，则开始循环，知道查到父节点的最后一个新增的孩子节点
				to_target=to_target->child;
				while(to_target->brother!=NULL)to_target=to_target->brother;
				to_target->brother=ptr;                 //将新增的节点与查找到的最后一个孩子节点连接起来
			}
		}
	}
	insert_hash(ptr);
	return base;
}

void delete_tree0(Tree*del){//后序递归删除
	if(del==NULL)return;
	delete_tree0(del->child);
	delete_tree0(del->brother);
	delete_hash(del);
	free(del);
	del=NULL;	
}

void delete_tree(Tree*del){//删除目标
	if(del==NULL)return;
	delete_tree0(del->child);
	delete_hash(del);
	free(del);
	del=NULL;
}

void delete_member(Tree** base){     //删除成员的函数
	printf("请输入删除成员的名称:");
	fgets(member,io_size,stdin);
	char*to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);

	 Tree *ptr2find,*ptr2del;
	 //ptr2find=delete_hash(member,alphabet);
	 ptr2find=find_member(member);
	 if(ptr2find==NULL){
		 printf("此人不存在");
		 return ;
	 }
	 ptr2del=ptr2find;
	 Tree *ptr2dad;
	 ptr2dad=ptr2find->parent;
	 
	if(ptr2dad==NULL){       //如果被删除节点是第一个，则释放
		 delete_tree(ptr2del);
		 ptr2del=NULL;
		 //delete_tree(base);
		 *base=NULL;
	}
	else{ 
		if(ptr2dad->child==ptr2find){    //判断被删除节点是否是父节点直接指向的节点
			//if(ptr2find->brother!=NULL)
			//{
				ptr2dad->child=ptr2find->brother;
			//}
			delete_tree (ptr2del);
		 ptr2del=NULL;
		}
		else{  //不是大儿子
			Tree *right=ptr2dad->child,*left;
			for(left=right;right!=NULL;left=right,right=right->brother){
				if(right==ptr2del){ 
					left->brother=right->brother;
					delete_tree(ptr2del);
					ptr2del=NULL;
					break;
				}
			}
		}
	}
}

void traverse(Tree *base){ //遍历  打印族谱
	if (base!=NULL){
		int i=0;   	
		for(i=1;i<base->level;i++)
			printf("    ");
		for_print(base,0);
		traverse(base->child);
		traverse(base->brother);
   }
}
	  
void revision(){       //修改成员信息的函数	
	Tree *ptr; char* to0;
	printf("请输入要修改的成员名称:  ");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';fflush(stdin);		
	ptr=find_member(member);                    //查找要修改的节点
	if(ptr==NULL){                                 //没查到，返回NULL
		printf("查无此人\n"); 
		return ;
	}
	delete_hash(ptr);
	printf("请输入修改信息,");               //查到之后进行逐项修改
	for_scan(ptr);
	insert_hash(ptr);
}
////////////////////////////////////////////////////////////////////////////////////////////////
Tree* common_ancestor_and_relationship(char left[],char right[],int *status){//查找最近共同祖先和三代关系
	hashtable*lefthash,*righthash;
	Tree*lowone,*highone;
	lefthash=search_hash(left);righthash=search_hash(right);
	if(lefthash==NULL||righthash==NULL)return NULL;
	lowone=lefthash->tree->level  >= righthash->tree->level?lefthash->tree:righthash->tree;
	highone=lefthash->tree->level < righthash->tree->level?lefthash->tree:righthash->tree;
	for(;lowone->level>highone->level;lowone=lowone->parent);
	for(;lowone!=highone;lowone=lowone->parent,highone=highone->parent);
	
	if(lowone==lefthash->tree&&lowone==righthash->tree){*status=3;}
	else if(lowone==lefthash->tree){*status=1;}
	else if(lowone==righthash->tree){*status=2;}
	
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==0){printf("你找自己干嘛？吃饱了没事干？\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==-1 || 
	lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==0){printf("直系，两人父母-子女关系\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==-2 || 
	lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==0){printf("直系，两人爷孙关系\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==-1 ){printf("旁系，两人兄弟姐妹关系\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==-2 ||
	lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==-1){printf("旁系，两人伯叔-侄甥子女关系\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==-2 ){printf("旁系，两人堂表兄弟姐妹关系\n");return 	lowone;};
	printf("两人不在直系旁系三代以内\n");	
	return 	lowone;
}

void output_common_ancestor(){//输出是否有共同祖先等
	char left[io_size],right[io_size];char* to0;
	int state=0;
	int *status=&state;
	printf("请输入第一个人\n");
	fgets(left,io_size,stdin);
	to0=NULL;to0 = strchr(left, '\n'); if(to0)*to0 = '\0';  fflush(stdin);
	printf("请输入第二个人\n");
	fgets(right,io_size,stdin);
	to0=NULL;to0 = strchr(right, '\n'); if(to0)*to0 = '\0';   fflush(stdin);
	Tree*ancestor=common_ancestor_and_relationship(left,right,status);
	if(ancestor){
		if(*status==0)printf("两人有共同祖先，最近的祖先的信息如下");
		else if(*status==1)printf("%s是%s的祖先,祖先的信息如下",left,right);
		else if(*status==2)printf("%s是%s的祖先,祖先的信息如下",right,left);
		else if(*status==3)printf("这是同一个人," );
		for_print(ancestor,0);
	}
	else printf("这两人或许五百年前是一家");
}

void level_order_traverse(Tree*bptr){//层次遍历并输出每一代
	Tree* queue[maxsize]={NULL},*temp;//int level=1;
	queue[0]=bptr;
	int front = 0, rear = 1;
	while (front<rear){
		if (queue[front]){
			//if(queue[front]->level>level){level=queue[front]->level;printf("");}
			int i=1;for(i=1;i<queue[front]->level;i++)printf("    ");
			for_print(queue[front],0);
			for(temp=queue[front]->child;temp!=NULL;temp=temp->brother){
				queue[rear++]=temp;
			}
			front++;
		}
		else{
			front++;
		}
	}
}

void traverse4son2(Tree* in){//输出所有2代内子孙
	Tree* temp=NULL,*temp1=NULL;
	if(in==NULL||in->child==NULL){printf("你在找空气吗？\n");return;}
	else{
		for(temp=in->child;temp!=NULL;temp=temp->brother){
			for_print(temp,0);
			for(temp1=temp->child;temp1!=NULL;temp1=temp1->brother){
				int i=0;for(i=1;i<temp1->level;i++)printf("    ");
				for_print(temp1,0);
			}
		}
	}
}

void traverse4cousins(Tree* in){//输出所有堂表兄弟
	if ((!in)||(!in->parent)||(!in->parent->parent)){printf("家谱内无此人表亲\n");return;}
	Tree* up=in->parent->parent,*temp,*temp1;
	for(temp=up->child;temp!=NULL;temp=temp->brother){
		for(temp1=temp->child;temp1!=NULL&&temp1!=in;temp1=temp1->brother){
			int i=0;for(i=1;i<temp1->level;i++)printf("    ");
			for_print(temp1,0);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SqString change_sign(char sign[]){          //将字符串转换成串存储结构
      int i;
      SqString str;
      for(i=0;sign[i]!='\0';i++){
         str.data[i]=sign[i];
	  }
      str.length=i;
      return str;
}

int match_up(SqString s,SqString t){          //BF算法 串的模式匹配,o(n^m)
    int i=0,j=0;
    while(i<s.length&&j<t.length){
      if(s.data[i]==t.data[j]){ //继续匹配下一个字符		  
         	i++; //主串和字串依次匹配下一个节点
        	j++;
		}
        else{
        	i=i-j+1;
		 	j=0;
	  }
	}
   if(j>=t.length)return true;// (i-t.length);下标
   else	return false;
}

void traverse8key(Tree *base, char aboutInformation[]){ //遍历搜索简历
    if(base!=NULL){		
		if(match_up(change_sign(base->name),change_sign(aboutInformation))==true&&match_up(change_sign(base->fed),change_sign(aboutInformation))==true){  
			printf("该家族成员的名字和配偶的名字都含有该关键字:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
		else if(match_up(change_sign(base->fed),change_sign(aboutInformation))==true){  
			printf("该家族成员的配偶名含有该关键字:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
		else if(match_up(change_sign(base->name),change_sign(aboutInformation))==true){  
			printf("该家族成员的名字中含有该关键字:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
	    traverse8key(base->child,aboutInformation); //进行递归调用，遍历每一个节点的个人简历
        traverse8key(base->brother,aboutInformation);			
	}
}

void in_key(Tree * u){                //根据关键字查询
	printf("请输入关键字：");
	char aboutInformation[io_size];char* to0;
	fgets(aboutInformation,io_size,stdin);
	to0=NULL;to0 = strchr(aboutInformation, '\n');if(to0)*to0 = '\0';   fflush(stdin);
    traverse8key(u,aboutInformation);     //将关键字传入进行遍历搜索
}
//////////////////////////////////////////////////////////////////////////////////////////////
void get_name(char changename[]){  //伪随机生成名字
	int i=0;
	for(i=0;i<3;i++)
		changename[i]='a'+(rand()%26);
	changename[i]='\0';	
	//printf("%s--\n",changename);
}

Tree** new_pointer(Tree**target,int option){	//生成树节点
	Tree*ptr;
	if(NULL==(ptr=(Tree*)malloc(sizeof(Tree)))){
		printf("内存不足\n");
		return NULL;}
	get_name(ptr->name);
	ptr->sex[0]='\0';
	get_name(ptr->fed);
	if(option==0){
	ptr->brother=NULL;                     
	ptr->child=NULL;                 
	ptr->parent=(*target);
	if((*target)==NULL)ptr->level=1;
	else ptr->level=(*target)->level+1; //记录辈分，为第一代
	}
	if(option==1){
		(*target)->child=ptr;
		ptr->brother=NULL;                     
		ptr->child=NULL;                
		ptr->parent=(*target);
		if((*target)==NULL)ptr->level=1;
		else ptr->level=(*target)->level+1; //记录辈分，为上一代加一
	}	
	if(option==2){
		(*target)->brother=ptr;
		ptr->brother=NULL;                     
		ptr->child=NULL;                
		ptr->parent=(*target)->parent;
		if((*target)==NULL)ptr->level=1;
		else ptr->level=(*target)->level; 	//记录辈分，为同一代
	}
	insert_hash(ptr);
	Tree**ptr4back;
		if(NULL==(ptr4back=(Tree**)malloc(sizeof(Tree)))){
		printf("内存不足\n");
		return NULL;}
	*ptr4back=ptr;
	
	return ptr4back;
}

Tree** loop4create(Tree**intree,int count4kid,int count4bro,int *limit,int trigger){//递归生成4代家谱
	Tree**ptr2son2;
		if(NULL==(ptr2son2=(Tree**)malloc(sizeof(Tree*)))){
		printf("内存不足\n");
		return NULL;}
	//printf("kid=%d,bro=%d---\n",count4kid,count4bro);			
	if(count4kid<*limit){
	ptr2son2=new_pointer(intree,1);
	//printf("这是新增的儿子，kid=%d,bro=%d---",count4kid,count4bro);for_print(*ptr2son2,1);
	if(trigger==0)	loop4create(ptr2son2,count4kid+1,count4bro,limit,0);	
	if(trigger==1)	loop4create(ptr2son2,count4kid+1,count4bro-1,limit,0);				
	}
	if(count4bro<*limit){
	ptr2son2=new_pointer(intree,2);		
	//printf("这是新增的兄弟，kid=%d,bro=%d---",count4kid,count4bro);for_print(*ptr2son2,1);	
	loop4create(ptr2son2,count4kid,count4bro+1,limit,1);
	}
	return ptr2son2;
}

Tree* init_pedigree(Tree*first){           //生成使用伪随机三字母作人名的4代家谱
	printf("此操作将会删除你之前输入的所有数据，再次确定按1，按其他键返回菜单  ");
	char option[2]={'\0','\0'},*to0;
	fgets(option,2,stdin);fflush(stdin);
	to0=NULL;to0 = strchr(option, '\n');if(to0)*to0 = '\0';
	if(option[0]!='1')return first;
	Tree*  *ptr2first;
		if(NULL==(ptr2first=(Tree**)malloc(sizeof(Tree)))){
		printf("内存不足\n");
		return NULL;}	
	delete_tree(first);//删除树和哈希表
	*ptr2first=NULL;
	first=*new_pointer(ptr2first,0);//新祖宗//删除后first好像是NULL//不传二级指针不会变null
	*ptr2first=first;//更新
	first->child=*new_pointer(ptr2first,1);
	Tree*  *ptr2fchild;
		if(NULL==(ptr2fchild=(Tree**)malloc(sizeof(Tree)))){
		printf("内存不足\n");
		return NULL;}	
	*ptr2fchild=first->child;
	int *upper=(int*)malloc(sizeof(int));*upper=3;
	loop4create(ptr2fchild,1,1,upper,0);
	return first;
}

int menu_select(){                             //菜单函数定义{
   char trigger[]={'\0','\0'};char* to0;
   printf("\n");
   //test_hash(alphabet);
   printf("=============================================================\n");
   printf("------------------家谱管理系统(内存版by蓝精灵)--------------\n");
   printf("                0.生成使用伪随机三字母作人名的4代家谱         \n");
   printf("                1.添加成员                                   \n");
   printf("                2.删除成员                          		\n");
   printf("                3.查询（配偶）名字含有关键字的人的信息     		\n");
   printf("                4.输出某人所有2代内子孙或所有堂表兄弟            \n");
   printf("                5.查找两人三代关系和最近共同祖先            \n");
   printf("                6.打印族谱树型结构                          		\n");
   printf("                7.层次遍历整个族谱       \n");
   printf("                8.修改信息   		\n");
   printf("                9.退出系统                          		\n");
   printf("**************************************************************\n");	
   printf("                    请输入（0-9）进行操作：\n");
   do{
		fgets(trigger,2,stdin);
		to0=NULL;to0 = strchr(trigger, '\n');if(to0)*to0 = '\0';  fflush(stdin);
   }while(trigger[0]<'0'||trigger[0]>'9');
   return trigger[0]-48;
}

int main(){
	//hashtable* alphabet[26]={NULL};
	Tree *u;char* to0;
	u=NULL;
	int n;		
	while(1){
		n=menu_select();//界面和功能选择模板
		//traverse(u);	
		if(n==0){
			u=init_pedigree(u);//生成使用伪随机三字母作人名的4代家谱
		}
		else if(n==1){
		 	u=add_member(u);//添加成员
		}
		else if(u==NULL){printf("请按下键盘1添加初代目或按下0生成预设家谱!");continue;}
		
		else if(n==2){ 			
			Tree** to_u=&u;
			delete_member(to_u);//删除成员
		}
		else if(n==3){
			in_key(u);//为递归查找输入关键字
		}
		else if(n==4){
			printf("请输入查询名称:"); 
			fgets(member,io_size,stdin);
			to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			Tree *intree=NULL;
			if(NULL==(intree=find_member(member))){
				printf("查无此人");continue;
			}
			int getinput=0;
			char trigger[]={'\0','\0'};	
			for(;trigger[0]!='1'&&trigger[0]!='2';){
				printf("输出所有2代内子孙请按1,输出所有堂表兄弟请按2\n"); 
					fgets(trigger,2,stdin);
					to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			}
			if(trigger[0]=='1')traverse4son2(intree);//输出所有2代内子孙
			if(trigger[0]=='2')traverse4cousins(intree);	//输出所有堂表兄弟			
		}
		else if(n==5){
			output_common_ancestor();//查找两人三代关系和最近共同祖先
		}
		else if(n==6){
			traverse(u);//先序遍历并打印族谱树型结构
		}
		else if(n==7){	
			level_order_traverse(u);//层次遍历并输出每一代
		}
		else if(n==8){
			revision();//修改成员信息
		}
		else break;
		}
	return 0;
}
