#include<stdio.h>
#include<String.h>
#include<stdlib.h> 
#define true 1
#define false 0
#define maxsize 1000
#define io_size 20

char member[20];

typedef struct{
	char data[maxsize]; //����ַ��� 
	int length; //��Ŵ���
}SqString;

typedef struct Tree{ 
	char name[io_size];           
	char sex[2];              
	char fed[io_size];//��ż
	int level;//����
	struct Tree *child;              //����ָ��
	struct Tree *brother;            //�ֵ�ָ��
	struct Tree *parent;             //����ָ��
}Tree;

typedef struct hashtable{
	char name[io_size];
	Tree * tree; 
	struct hashtable* next; 
}hashtable; 

	hashtable* alphabet[26]={NULL};//��ȫ�־����ڲ���صĺ������޸��ˣ����������β�����һ����ûʲô��

void for_print(Tree* base,int status);

int locate_hash(char s[]){//������ĩ�ַ���λ��ϣ��
	int i=io_size-2;
	for(i=0;s[i]!='\0';i++);i--;	
	if(s[i]>='A'&&s[i]<='Z')return (s[i]-'A')%26;
	if(s[i]>='a'&&s[i]<='z')return (s[i]-'a')%26;
	else return s[i]%26;
}

void insert_hash(Tree* pedigree){//�����ϣ��
	int p; hashtable* np;
	p=locate_hash(pedigree->name);
	np=(hashtable*)malloc(sizeof(hashtable));
	if(NULL==np){
		printf("�ڴ治��\n");
		return ;}
	strcpy(np->name,pedigree->name);
	np->next=alphabet[p];
	np->tree=pedigree;
	alphabet[p]=np;	
}

hashtable* search_hash(char s[]){//�ڹ�ϣ���в��ң���Ҫ��Ϊ��Ӧ����������
	hashtable* arrays[maxsize]={NULL};
	int p=locate_hash(s),i=-1,j=0;
	hashtable* np;

	for(np=alphabet[p];np!=NULL;np=np->next)
		if(strcmp(np->name,s)==0){arrays[++i]=np;}
	if(i==-1)return NULL;
	if(i>=1){
		printf("���¼���ѡ����\n");
		for(j=0;j<=i;j++){
			printf("���%d---",j);for_print(arrays[j]->tree,1);
		}
		scanf("%d",&j);getchar();
		//printf("u chose NO.%d\n",j);		
	}
	return arrays[j];
}

void delete_hash(Tree* del){//�ڹ�ϣ����ɾ��
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

void test_hash(){//���Թ�ϣ��������
	int i=0;hashtable* np;
	for (;i<26;i++){
		for(np=alphabet[i];np!=NULL;np=np->next)printf("alphabet[%d]=%s\n",i,np->name);	
	}
}

void for_print(Tree* base,int status){//��ӡ��Ա��Ϣ��ͳһ����
	if(base==NULL){printf("�Ա���û�¸ɣ�\n");return;}
	else{
		if(status==1){
			if(base->parent==NULL)printf("���ײ���---");
			else printf("������%s---",base->parent->name);
		}
		printf("����:��%d��---",base->level);
		printf("�����֡�:%s---",base->name);
		if(base->sex[0]=='\0')printf("�Ա�:��---");else printf("�Ա�:Ů---");																			
     	if(base->fed[0]=='\0')printf("��ż:�޻���---\n");else printf("��ż:%s---\n",base->fed);
	}
}

int for_scan(Tree*base){//�����Ա��Ϣ��ͳһ����
	char*to0;
	printf("����:");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->name,member);
	if('\0'==base->name[0])return 0;	
	printf("�Ա�(ֱ�ӻس�Ĭ����,�����������Ů):");
	fgets(member,2,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->sex,member);
	printf("��ż����(ֱ�ӻس�Ĭ���ޣ����������������):");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n'); if(to0)*to0 = '\0'; fflush(stdin);
	strcpy(base->fed,member);
	
	return 1;
}

Tree *find_member(char cname[]){//���ӹ�ϣ�����������

	hashtable*temp=search_hash(cname);
	if(temp==NULL)return NULL;
	return temp->tree;	
}

Tree* add_member(Tree *base){ //��ӳ�Ա
	char option[2]={'\0','\0'};char* to0;
	printf("��ӳ�Ա:");
	Tree *ptr,*to_target; 
	if(NULL==(ptr=(Tree*)malloc(sizeof(Tree)))){
		printf("�ڴ治��\n");
		return NULL;}
	
	if(0==for_scan(ptr))return base;
	
	if(base==NULL){
	printf("���ǳ���Ŀ��\n");
	ptr->brother=NULL;                     
	ptr->child=NULL;     
	ptr->parent=NULL;
	ptr->level=1; 		//��¼���֣�Ϊ��һ��
	base=ptr;
	}
	else{
		printf("������Ҫ��ӹ�ϵ��Ŀ���Ա������:");
		fgets(member,io_size,stdin);
		to0=NULL;to0=strchr(member,'\n');if(to0)*to0 = '\0';fflush(stdin);
		to_target=find_member(member);
		if(NULL==to_target){printf("Ŀ�겻����");return base;}	
		else {
			if(to_target!=base){
				printf("�ýڵ㲻�����ڣ���Ӹýڵ��  1:�ֵ�, ������(�����س�):����    "); 
				fgets(option,2,stdin);
				to0=NULL;to0 = strchr(option, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			}	
			if(to_target!=base&&option[0]=='1'){//�����ֵ�
				printf("��ӵ���Ŀ����ֵ�\n");
				for(;to_target->brother!=NULL;to_target=to_target->brother);
				ptr->level=to_target->level;   
				ptr->child=NULL;                
				ptr->brother=NULL;               
				ptr->parent=to_target->parent; 
				to_target->brother=ptr;
				insert_hash(ptr);
				return base;
			}
			printf("��ӵ���Ŀ��Ķ���\n");
			ptr->level=to_target->level+1;              
			ptr->child=NULL;                  
			ptr->brother=NULL;                
			ptr->parent=to_target;                    //���׽����Ǵ˺����տ�ʼʱ�Ĳ��ҵ��ģ�Ȼ����и�ֵ
			if(to_target->child==NULL)to_target->child=ptr;
			else{                         //����к��ӣ���ʼѭ����֪���鵽���ڵ�����һ�������ĺ��ӽڵ�
				to_target=to_target->child;
				while(to_target->brother!=NULL)to_target=to_target->brother;
				to_target->brother=ptr;                 //�������Ľڵ�����ҵ������һ�����ӽڵ���������
			}
		}
	}
	insert_hash(ptr);
	return base;
}

void delete_tree0(Tree*del){//����ݹ�ɾ��
	if(del==NULL)return;
	delete_tree0(del->child);
	delete_tree0(del->brother);
	delete_hash(del);
	free(del);
	del=NULL;	
}

void delete_tree(Tree*del){//ɾ��Ŀ��
	if(del==NULL)return;
	delete_tree0(del->child);
	delete_hash(del);
	free(del);
	del=NULL;
}

void delete_member(Tree** base){     //ɾ����Ա�ĺ���
	printf("������ɾ����Ա������:");
	fgets(member,io_size,stdin);
	char*to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);

	 Tree *ptr2find,*ptr2del;
	 //ptr2find=delete_hash(member,alphabet);
	 ptr2find=find_member(member);
	 if(ptr2find==NULL){
		 printf("���˲�����");
		 return ;
	 }
	 ptr2del=ptr2find;
	 Tree *ptr2dad;
	 ptr2dad=ptr2find->parent;
	 
	if(ptr2dad==NULL){       //�����ɾ���ڵ��ǵ�һ�������ͷ�
		 delete_tree(ptr2del);
		 ptr2del=NULL;
		 //delete_tree(base);
		 *base=NULL;
	}
	else{ 
		if(ptr2dad->child==ptr2find){    //�жϱ�ɾ���ڵ��Ƿ��Ǹ��ڵ�ֱ��ָ��Ľڵ�
			//if(ptr2find->brother!=NULL)
			//{
				ptr2dad->child=ptr2find->brother;
			//}
			delete_tree (ptr2del);
		 ptr2del=NULL;
		}
		else{  //���Ǵ����
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

void traverse(Tree *base){ //����  ��ӡ����
	if (base!=NULL){
		int i=0;   	
		for(i=1;i<base->level;i++)
			printf("    ");
		for_print(base,0);
		traverse(base->child);
		traverse(base->brother);
   }
}
	  
void revision(){       //�޸ĳ�Ա��Ϣ�ĺ���	
	Tree *ptr; char* to0;
	printf("������Ҫ�޸ĵĳ�Ա����:  ");
	fgets(member,io_size,stdin);
	to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';fflush(stdin);		
	ptr=find_member(member);                    //����Ҫ�޸ĵĽڵ�
	if(ptr==NULL){                                 //û�鵽������NULL
		printf("���޴���\n"); 
		return ;
	}
	delete_hash(ptr);
	printf("�������޸���Ϣ,");               //�鵽֮����������޸�
	for_scan(ptr);
	insert_hash(ptr);
}
////////////////////////////////////////////////////////////////////////////////////////////////
Tree* common_ancestor_and_relationship(char left[],char right[],int *status){//���������ͬ���Ⱥ�������ϵ
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
	
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==0){printf("�����Լ�����Ա���û�¸ɣ�\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==-1 || 
	lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==0){printf("ֱϵ�����˸�ĸ-��Ů��ϵ\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==0 && lowone->level - righthash->tree->level==-2 || 
	lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==0){printf("ֱϵ������ү���ϵ\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==-1 ){printf("��ϵ�������ֵܽ��ù�ϵ\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-1 && lowone->level - righthash->tree->level==-2 ||
	lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==-1){printf("��ϵ�����˲���-ֶ����Ů��ϵ\n");return 	lowone;};
	if(lowone->level - lefthash->tree->level==-2 && lowone->level - righthash->tree->level==-2 ){printf("��ϵ�������ñ��ֵܽ��ù�ϵ\n");return 	lowone;};
	printf("���˲���ֱϵ��ϵ��������\n");	
	return 	lowone;
}

void output_common_ancestor(){//����Ƿ��й�ͬ���ȵ�
	char left[io_size],right[io_size];char* to0;
	int state=0;
	int *status=&state;
	printf("�������һ����\n");
	fgets(left,io_size,stdin);
	to0=NULL;to0 = strchr(left, '\n'); if(to0)*to0 = '\0';  fflush(stdin);
	printf("������ڶ�����\n");
	fgets(right,io_size,stdin);
	to0=NULL;to0 = strchr(right, '\n'); if(to0)*to0 = '\0';   fflush(stdin);
	Tree*ancestor=common_ancestor_and_relationship(left,right,status);
	if(ancestor){
		if(*status==0)printf("�����й�ͬ���ȣ���������ȵ���Ϣ����");
		else if(*status==1)printf("%s��%s������,���ȵ���Ϣ����",left,right);
		else if(*status==2)printf("%s��%s������,���ȵ���Ϣ����",right,left);
		else if(*status==3)printf("����ͬһ����," );
		for_print(ancestor,0);
	}
	else printf("�����˻��������ǰ��һ��");
}

void level_order_traverse(Tree*bptr){//��α��������ÿһ��
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

void traverse4son2(Tree* in){//�������2��������
	Tree* temp=NULL,*temp1=NULL;
	if(in==NULL||in->child==NULL){printf("�����ҿ�����\n");return;}
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

void traverse4cousins(Tree* in){//��������ñ��ֵ�
	if ((!in)||(!in->parent)||(!in->parent->parent)){printf("�������޴��˱���\n");return;}
	Tree* up=in->parent->parent,*temp,*temp1;
	for(temp=up->child;temp!=NULL;temp=temp->brother){
		for(temp1=temp->child;temp1!=NULL&&temp1!=in;temp1=temp1->brother){
			int i=0;for(i=1;i<temp1->level;i++)printf("    ");
			for_print(temp1,0);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SqString change_sign(char sign[]){          //���ַ���ת���ɴ��洢�ṹ
      int i;
      SqString str;
      for(i=0;sign[i]!='\0';i++){
         str.data[i]=sign[i];
	  }
      str.length=i;
      return str;
}

int match_up(SqString s,SqString t){          //BF�㷨 ����ģʽƥ��,o(n^m)
    int i=0,j=0;
    while(i<s.length&&j<t.length){
      if(s.data[i]==t.data[j]){ //����ƥ����һ���ַ�		  
         	i++; //�������ִ�����ƥ����һ���ڵ�
        	j++;
		}
        else{
        	i=i-j+1;
		 	j=0;
	  }
	}
   if(j>=t.length)return true;// (i-t.length);�±�
   else	return false;
}

void traverse8key(Tree *base, char aboutInformation[]){ //������������
    if(base!=NULL){		
		if(match_up(change_sign(base->name),change_sign(aboutInformation))==true&&match_up(change_sign(base->fed),change_sign(aboutInformation))==true){  
			printf("�ü����Ա�����ֺ���ż�����ֶ����иùؼ���:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
		else if(match_up(change_sign(base->fed),change_sign(aboutInformation))==true){  
			printf("�ü����Ա����ż�����иùؼ���:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
		else if(match_up(change_sign(base->name),change_sign(aboutInformation))==true){  
			printf("�ü����Ա�������к��иùؼ���:\n");
			int i=0;for(i=1;i<base->level;i++)printf("      ");
			for_print(base,0);
		}
	    traverse8key(base->child,aboutInformation); //���еݹ���ã�����ÿһ���ڵ�ĸ��˼���
        traverse8key(base->brother,aboutInformation);			
	}
}

void in_key(Tree * u){                //���ݹؼ��ֲ�ѯ
	printf("������ؼ��֣�");
	char aboutInformation[io_size];char* to0;
	fgets(aboutInformation,io_size,stdin);
	to0=NULL;to0 = strchr(aboutInformation, '\n');if(to0)*to0 = '\0';   fflush(stdin);
    traverse8key(u,aboutInformation);     //���ؼ��ִ�����б�������
}
//////////////////////////////////////////////////////////////////////////////////////////////
void get_name(char changename[]){  //α�����������
	int i=0;
	for(i=0;i<3;i++)
		changename[i]='a'+(rand()%26);
	changename[i]='\0';	
	//printf("%s--\n",changename);
}

Tree** new_pointer(Tree**target,int option){	//�������ڵ�
	Tree*ptr;
	if(NULL==(ptr=(Tree*)malloc(sizeof(Tree)))){
		printf("�ڴ治��\n");
		return NULL;}
	get_name(ptr->name);
	ptr->sex[0]='\0';
	get_name(ptr->fed);
	if(option==0){
	ptr->brother=NULL;                     
	ptr->child=NULL;                 
	ptr->parent=(*target);
	if((*target)==NULL)ptr->level=1;
	else ptr->level=(*target)->level+1; //��¼���֣�Ϊ��һ��
	}
	if(option==1){
		(*target)->child=ptr;
		ptr->brother=NULL;                     
		ptr->child=NULL;                
		ptr->parent=(*target);
		if((*target)==NULL)ptr->level=1;
		else ptr->level=(*target)->level+1; //��¼���֣�Ϊ��һ����һ
	}	
	if(option==2){
		(*target)->brother=ptr;
		ptr->brother=NULL;                     
		ptr->child=NULL;                
		ptr->parent=(*target)->parent;
		if((*target)==NULL)ptr->level=1;
		else ptr->level=(*target)->level; 	//��¼���֣�Ϊͬһ��
	}
	insert_hash(ptr);
	Tree**ptr4back;
		if(NULL==(ptr4back=(Tree**)malloc(sizeof(Tree)))){
		printf("�ڴ治��\n");
		return NULL;}
	*ptr4back=ptr;
	
	return ptr4back;
}

Tree** loop4create(Tree**intree,int count4kid,int count4bro,int *limit,int trigger){//�ݹ�����4������
	Tree**ptr2son2;
		if(NULL==(ptr2son2=(Tree**)malloc(sizeof(Tree*)))){
		printf("�ڴ治��\n");
		return NULL;}
	//printf("kid=%d,bro=%d---\n",count4kid,count4bro);			
	if(count4kid<*limit){
	ptr2son2=new_pointer(intree,1);
	//printf("���������Ķ��ӣ�kid=%d,bro=%d---",count4kid,count4bro);for_print(*ptr2son2,1);
	if(trigger==0)	loop4create(ptr2son2,count4kid+1,count4bro,limit,0);	
	if(trigger==1)	loop4create(ptr2son2,count4kid+1,count4bro-1,limit,0);				
	}
	if(count4bro<*limit){
	ptr2son2=new_pointer(intree,2);		
	//printf("�����������ֵܣ�kid=%d,bro=%d---",count4kid,count4bro);for_print(*ptr2son2,1);	
	loop4create(ptr2son2,count4kid,count4bro+1,limit,1);
	}
	return ptr2son2;
}

Tree* init_pedigree(Tree*first){           //����ʹ��α�������ĸ��������4������
	printf("�˲�������ɾ����֮ǰ������������ݣ��ٴ�ȷ����1�������������ز˵�  ");
	char option[2]={'\0','\0'},*to0;
	fgets(option,2,stdin);fflush(stdin);
	to0=NULL;to0 = strchr(option, '\n');if(to0)*to0 = '\0';
	if(option[0]!='1')return first;
	Tree*  *ptr2first;
		if(NULL==(ptr2first=(Tree**)malloc(sizeof(Tree)))){
		printf("�ڴ治��\n");
		return NULL;}	
	delete_tree(first);//ɾ�����͹�ϣ��
	*ptr2first=NULL;
	first=*new_pointer(ptr2first,0);//������//ɾ����first������NULL//��������ָ�벻���null
	*ptr2first=first;//����
	first->child=*new_pointer(ptr2first,1);
	Tree*  *ptr2fchild;
		if(NULL==(ptr2fchild=(Tree**)malloc(sizeof(Tree)))){
		printf("�ڴ治��\n");
		return NULL;}	
	*ptr2fchild=first->child;
	int *upper=(int*)malloc(sizeof(int));*upper=3;
	loop4create(ptr2fchild,1,1,upper,0);
	return first;
}

int menu_select(){                             //�˵���������{
   char trigger[]={'\0','\0'};char* to0;
   printf("\n");
   //test_hash(alphabet);
   printf("=============================================================\n");
   printf("------------------���׹���ϵͳ(�ڴ��by������)--------------\n");
   printf("                0.����ʹ��α�������ĸ��������4������         \n");
   printf("                1.��ӳ�Ա                                   \n");
   printf("                2.ɾ����Ա                          		\n");
   printf("                3.��ѯ����ż�����ֺ��йؼ��ֵ��˵���Ϣ     		\n");
   printf("                4.���ĳ������2��������������ñ��ֵ�            \n");
   printf("                5.��������������ϵ�������ͬ����            \n");
   printf("                6.��ӡ�������ͽṹ                          		\n");
   printf("                7.��α�����������       \n");
   printf("                8.�޸���Ϣ   		\n");
   printf("                9.�˳�ϵͳ                          		\n");
   printf("**************************************************************\n");	
   printf("                    �����루0-9�����в�����\n");
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
		n=menu_select();//����͹���ѡ��ģ��
		//traverse(u);	
		if(n==0){
			u=init_pedigree(u);//����ʹ��α�������ĸ��������4������
		}
		else if(n==1){
		 	u=add_member(u);//��ӳ�Ա
		}
		else if(u==NULL){printf("�밴�¼���1��ӳ���Ŀ����0����Ԥ�����!");continue;}
		
		else if(n==2){ 			
			Tree** to_u=&u;
			delete_member(to_u);//ɾ����Ա
		}
		else if(n==3){
			in_key(u);//Ϊ�ݹ��������ؼ���
		}
		else if(n==4){
			printf("�������ѯ����:"); 
			fgets(member,io_size,stdin);
			to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			Tree *intree=NULL;
			if(NULL==(intree=find_member(member))){
				printf("���޴���");continue;
			}
			int getinput=0;
			char trigger[]={'\0','\0'};	
			for(;trigger[0]!='1'&&trigger[0]!='2';){
				printf("�������2���������밴1,��������ñ��ֵ��밴2\n"); 
					fgets(trigger,2,stdin);
					to0=NULL;to0 = strchr(member, '\n');if(to0)*to0 = '\0';  fflush(stdin);
			}
			if(trigger[0]=='1')traverse4son2(intree);//�������2��������
			if(trigger[0]=='2')traverse4cousins(intree);	//��������ñ��ֵ�			
		}
		else if(n==5){
			output_common_ancestor();//��������������ϵ�������ͬ����
		}
		else if(n==6){
			traverse(u);//�����������ӡ�������ͽṹ
		}
		else if(n==7){	
			level_order_traverse(u);//��α��������ÿһ��
		}
		else if(n==8){
			revision();//�޸ĳ�Ա��Ϣ
		}
		else break;
		}
	return 0;
}
