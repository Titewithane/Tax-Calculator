/*
    total.txt คือ ไฟล์รวม มีบอก tax(ภาษีที่ต้องจ่าย) netincome(เงินได้สุทธิ) status(สถานะ) name(ชื่อ)
    status เขียนย่อเอาเพราะเขียนเต็มรันออกมามันเละ
        sing = single
        divo = divorce
        cphs = couplehasincome
        cphn = couplehasnoincome
   00000 กดรันจะมี 4 เมนู
    1.tax calculator คำนวณภาษีออกรายงาน
    2.Edit พิมข้อมูลทับลงบรรทัดเดิมใน total.txt แล้วก็ออกรายงานใหม่
    3.Browsing ถามชื่อ แล้วถามว่าจะเปิดรายงาน หรือ เปิด total.txt
    4.delete ลบข้อมูลของ user คนนั้นในไฟล์ total.txt และ ลบไฟล์ report ของคนนั้นออก
    5.exit ออกจากโปรแกรม
*/


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

//global variable.
int data[100]; //data[20] is checker , if value of checker is 2 does mean net income = 0 and tax = 0
char state[100];

//function prototype.
int mistakenumber(char income[],char condition[],int boundaries);
void personal_discount(int *discount,int income);
void insurance(int *discount,int income);
void fund(int *discount,int income);
int netincomee(int *discount,int income);
int donate(int netincome,int income,int *discount);
int rateoftax(int netincome,int otherincome);
int changeschartoint(char number[]);
void inputname(char name[]);
int expense(int income);


int main(void){
    int choice,count = 0,check,income,otherincome,discount = 0,net,don,tax;
    char schoice[255],scheck[255],sincome[255],sotherincome[255],checker,backmenu;
    char name[100];
    FILE *total,*report;
    if((total = fopen("total.txt","r")) == NULL){
        total = fopen("total.txt","a");
        fprintf(total,"     Tax                            Netincome                    Status\t                  Name\n");
        fprintf(total,"---------------------------------------------------------------------------------------------------------------------\n");
        fclose(total);
    }
    else fclose(total);
    do{
    system("cls");
    printf("    Menu\n");
    printf("1.Tax calculator\n"); //คำนวณภาษี และ ออกรายงาน
    printf("2.Edit\n"); //แก้ไขข้อมูลและเขียนทับลงไปใน total.txt
    printf("3.Browsing\n"); //เรียกดู total.txt
    printf("4.Delete\n");  //ลบบรรทัด user คนนั้น
    printf("5.Exit\n");
    do{
        if(count > 0) printf("Error!!! fill out again.\n");
        printf(">> ");
        scanf("%s",schoice);
        choice = changeschartoint(schoice);
        count++;
    }while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5); count = 0;
    if(choice == 1 || choice == 2){
        system("cls");
        if(choice == 1) printf("1.Tax calculator\n");
        else if(choice == 2) printf("2.Edit\n");
        inputname(name);
        if(choice == 2){
            FILE *fptr1;
            int line1 = -1,col1 = -1;
            char strline1[255];
            char *pos1;
            if((total = fopen("total.txt","r")) == NULL){
                printf("Unable to open the input file!!\n");
                return 1;
            }
            while((fgets(strline1,255,total)) != NULL){
                line1++;
                pos1 = strstr(strline1,name);
                if(pos1 != NULL){
                    col1 = (pos1 - strline1);
                    break;
                }
            }
            fclose(total);
            if(col1 == -1) line1 = -1;
            if(line1 != -1) line1++;
            else {
                printf("%s does not exists.\n",name);
                return 1;
            }
        }
        printf("Income(salary + bonus)\n>> ");
        scanf("%s",sincome);
        income = changeschartoint(sincome);
        printf("Otherincome\n>> ");
        scanf("%s",sotherincome);
        otherincome = changeschartoint(sotherincome);
            system("cls");
        personal_discount(&discount,income);
            system("cls");
        insurance(&discount,income);
            system("cls");
        fund(&discount,income);
            system("cls");
        net = netincomee(&discount,income);
        don = donate(net,income,&discount);
        net += don;
        tax = rateoftax(net,otherincome);
        char namereport[100];
        strcpy(namereport,name);
        strcat(namereport,".txt");
        report = fopen(namereport,"w+");
        fprintf(report,"                      Tax calculate\n");
        fprintf(report,"-----------------------------------------------------------\n");
        fprintf(report,"Name : %s\n",name);
        fprintf(report,"Taxpayer Status                                    %s\n",state);
        fprintf(report,"1.income (after discount from expense)         %10d\n",income * 12-data[12]);
        fprintf(report,"2.allowance                                    %10d\n",discount-data[13]-data[14]);
        fprintf(report,"3.remaining (1. - 2.)                          %10d\n",income * 12-data[12]-(discount - data[13] - data[14]));
        fprintf(report,"4.deduction (Donation for scholarships)        %10d\n",data[13]);
        fprintf(report,"5.remaining (3. - 4.)                          %10d\n",income * 12-data[12]-discount-data[13]);
        fprintf(report,"6.deduction (General donation)                 %10d\n",data[14]);
        fprintf(report,"7.remaining (5. - 6.)                          %10d\n",income * 12-data[12]-discount-data[13]-data[14]);
        fprintf(report,"8.Tax (calculate from 7.)                      %10d\n",data[15]);
        fprintf(report,"9.Tax (calculate from otherincome)             %10d\n",data[16]);
        if(data[15] >= data[16]){
            fprintf(report,"10.Total tax                                   %10d\n",data[15]);
        }
        else if(data[15] < data[16]){
            fprintf(report,"10.Total tax                                   %10d\n",data[16]);
        }
        fclose(report);
        //show on console.
            system("cls");
        printf("                      Tax calculate\n");
        printf("----------------------------------------------------------------\n");
        printf("Name : %s\n",name);
        printf("Taxpayer Status                                    %s\n",state);
        printf("1.income (after discount from expense)         %10d\n",income * 12-data[12]);
        printf("2.allowance                                    %10d\n",discount-data[13]-data[14]);
        printf("3.remaining (1. - 2.)                          %10d\n",income * 12-data[12]-(discount - data[13] - data[14]));
        printf("4.deduction (Donation for scholarships)        %10d\n",data[13]);
        printf("5.remaining (3. - 4.)                          %10d\n",income * 12-data[12]-discount-data[13]);
        printf("6.deduction (General donation)                 %10d\n",data[14]);
        printf("7.remaining (5. - 6.)                          %10d\n",income * 12-data[12]-discount-data[13]-data[14]);
        printf("8.Tax (calculate from 7.)                      %10d\n",data[15]);
        printf("9.Tax (calculate from otherincome)             %10d\n",data[16]);
        if(data[15] >= data[16]){
            printf("10.Total tax                                   %10d\n",data[15]);
        }
        else if(data[15] < data[16]){
            printf("10.Total tax                                   %10d\n",data[16]);
        }
        if(choice == 1){
            /*
                sing is single.
                divo is divorce.
                cphs is couplehasincome.
                cphn is couplehasnoincome.
            */
            total = fopen("total.txt","a");
            if(strcmp(state,"single") == 0)
                fprintf(total,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"sing",name);
            else if(strcmp(state,"divorce") == 0)
                fprintf(total,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"divo",name);
            else if(strcmp(state,"couplehasincome") == 0)
                fprintf(total,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"cphs",name);
            else if(strcmp(state,"couplehasnoincome") == 0)
                fprintf(total,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"cphn",name);
            fclose(total);
        }
        //edit 
        else if(choice == 2){
            //replace data.
            char nfptr[] = "text.txt";
            FILE *fptr;
            int line = -1,col = -1,linectr = 0,lno;
            char strline[255],strtotaloneline[255],st[255],fname[255],newln[255];
            char *pos;
            if((total = fopen("total.txt","r")) == NULL){
                printf("Unable to open the input file!!\n");
                return 1;
            }
            while((fgets(strline,255,total)) != NULL){
                line++;
                pos = strstr(strline,name);
                if(pos != NULL){
                    col = (pos - strline);
                    break;
                }
            }
            if(col == -1) line = -1;
            if(line != -1) line++;
            else printf("%s does not exists.",name);
            fclose(total);
            if((total = fopen("total.txt","r")) == NULL){
                printf("Unable to open file.\n");
                return 1;
            }
            if((fptr = fopen(nfptr,"w+")) == NULL){
                printf("Unable to open file.\n");
                return 1;
            }
            while(!feof(total)){
                strcpy(strtotaloneline,"\0");
                fgets(strtotaloneline,255,total);
                if(!feof(total)){
                    linectr++;
                    if(linectr != line){
                        fprintf(fptr,"%s",strtotaloneline);
                    }
                    else{
                        if(strcmp(state,"single") == 0)
                            fprintf(fptr,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"sing",name);
                        else if(strcmp(state,"divorce") == 0)
                            fprintf(fptr,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"divo",name);
                        else if(strcmp(state,"couplehasincome") == 0)
                            fprintf(fptr,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"cphs",name);
                        else if(strcmp(state,"couplehasnoincome") == 0)
                            fprintf(fptr,"%10d                       %10d                      %s                     %s\n",rateoftax(net,otherincome),net,"cphn",name);
                    }
                }
            }
            printf("edit successfully.\n");
            fclose(fptr);
            fclose(total);
            remove("total.txt");
            rename(nfptr,"total.txt");
        }
    }
    else if(choice == 3){
        FILE *fptr;
        int choose,line = -1,col = -1,linectr = 0,lno;
        char string,schoose[100],strline[255],strtotaloneline[255],stringwholeline[255];
        char *pos;
        system("cls");
        printf("    Browsing\n");
        printf("1.report\n");
        printf("2.total\n");
        do{
            if(count > 0) printf("Error!!! fill out again.\n");
            printf(">> ");
            scanf("%s",schoose);
            choose = changeschartoint(schoose);
            count++;
        }while(choose != 1 && choose != 2); count = 0;
        inputname(name);
        char namereport[100];
        strcpy(namereport,name);
        strcat(namereport,".txt");
        if(choose == 1){
            if((report = fopen(namereport,"r")) == NULL){
                printf("Unable to open file.\n");
                return 1;
            }
            string = fgetc(report);
            while(string != EOF){
                printf("%c",string);
                string = fgetc(report);
            }
            fclose(report);
        }
        else if(choose == 2){
            if((total = fopen("total.txt","r")) == NULL){
                printf("Unable to open file.\n");
                return 1;
            }
            while((fgets(strline,255,total)) != NULL){
                line++;
                pos = strstr(strline,name);
                if(pos != NULL){
                    col = (pos - strline);
                    break;
                }
            }
            if(col == -1) line = -1;
            if(line != -1) line++;
            else printf("%s does not exists.",name);
            //printf("%s was found at line %d\n",name,line);
            fclose(total);
            //first i have a line to print....
            if((total = fopen("total.txt","r")) == NULL){
                printf("Unable to open file.\n");
                return 1;
            }
            if(total != NULL){
                int counter = 1;
                while(fgets(stringwholeline,255,total) != NULL){
                    if(counter == line){
                        if(counter == line){
                            system("cls");
                            printf("     Tax                            Netincome                    Status\t                   Name\n");
                            printf("-------------------------------------------------------------------------------------------------------------------------\n");
                            printf("%s\n",stringwholeline);
                        }
                    }
                    else counter++;
                }
            }
            fclose(total);
        }
    }
    else if(choice == 4){
        //delete certain file in line.
        FILE *fptr;
        int choose,line = -1,col = -1,linectr = 0,lno;
        char string,schoose[100],strline[255],strtotaloneline[255],stringwholeline[255],nfptr[] = "text.txt";
        char *pos;
        system("cls");
        printf("4.Delete\n");
        inputname(name);
        if((total = fopen("total.txt","r")) == NULL){
            printf("Unable to open file.\n");
            return 1;
        }
        while((fgets(strline,255,total)) != NULL){
            line++;
            pos = strstr(strline,name);
            if(pos != NULL){
                col = (pos - strline);
                break;
            }
        }
        if(col == -1) line = -1;
        if(line != -1) line++;
        else{
            printf("%s does not exists.",name);
            fclose(total);
            return 1;
        }
        //printf("%s was found at line %d\n",name,line);
        fclose(total);
        if((total = fopen("total.txt","r")) == NULL){
            printf("Unable to open file.\n");
            return 1;
        }
        if((fptr = fopen(nfptr,"w")) == NULL){
            printf("Unable to open file.\n");
            return 1;
        }
        int temp = 1;
        while((fgets(stringwholeline,255,total))){
            if(temp != line){
                fprintf(fptr,"%s",stringwholeline);
            }
            temp++;
        }
        fclose(total);
        fclose(fptr);
        remove("total.txt");
        rename(nfptr,"total.txt");
        printf("Delete Successful.");
        char strc[100];
        strcpy(strc,name);
        strcat(strc,".txt");
        remove(strc);
    }
    else if(choice == 5){
        exit(0);
    }
    do{
        printf("Do you want to go back to the menu?(Y/N)\n>> ");
        scanf(" %c",&backmenu);
        backmenu = tolower(backmenu);
    }while(backmenu != 'n' && backmenu != 'y');
    }while(backmenu != 'n');
    return 0;
}


int changeschartoint(char number[]){
    char dict[10] = "0123456789";
    int integer = 0,count,flags = 0;
    while(1){
        count = 0;
        if(flags > 0){
            printf("ERROR!!!! fill out again.\n");
            printf(">> ");
            scanf("%s",number);
        }
        for(int i = 0;i<strlen(number);i++){
            if(!isdigit(number[i]))
                count = 1;
        }
        if(count == 0) break;
        flags++;
    }
    int len = strlen(number) - 1;
    for(int i = 0;i<=strlen(number);i++){
        for(int j = 0;j<10;j++){
            if(number[i] == dict[j]){
                integer += j*pow(10,len); break;
            }
        } len--;
    }
    return integer;
}

//กรณีใส่เลขมากกว่าขอบเขต หรือ น้อยกว่า 0
int mistakenumber(char income[],char condition[],int boundaries){
    int number;
    char snumber[255];
    do{
        printf("%s(%s)\n>> ",income,condition);
        scanf("%s",snumber);
        number = changeschartoint(snumber);
        if(number > boundaries)
            printf("Please fill out again.\n");
    }while(number > boundaries);
    return number;
}

//ค่าลดหย่อนส่วนบุคคล
void personal_discount(int *discount,int income){
    char status[100];
    char dict[4][100] = {"single","divorce","couplehasincome","couplehasnoincome"};
    int i,number_sons,number_parents,number_disabled,count = 0,number_sonsafter,number_disabledspouse,number_parentsspouse;
    char snumber_sons[100],snumber_sonsafter[100],snumber_parents[100],snumber_disabled[100],snumber_disabledspouse[100],snumber_parentsspouse[100],yn;
    do{
        if(count > 0) printf("Please fill out again.\n");
        printf("Status(single,divorce,couplehasincome,couplehasnoincome)\n>> ");
        scanf("%s",status);
        for(int j = 0;j<4;j++){
            if(strcmp(status,dict[j]) == 0) goto label;
        }
        count++;
    }while(1);
    label :
    count = 0;
    strcpy(state,status);
    if(strcmp(status,dict[0]) == 0) /*โสด*/{
        //printf("discount = %d",*discount);
        *discount += 60000; //personal allowance
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father or Mother = 1 Father&Mother = 2 skip = 0)\n>> ");
            scanf("%s",snumber_parents);
            number_parents = changeschartoint(snumber_parents);
            if(number_parents == 1) *discount += 30000;
            else if(number_parents == 2) *discount += 60000;
            count++;
        }while(number_parents > 2 || number_parents < 0); count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 3 father mother and other.))\n>> ");
            scanf("%s",snumber_disabled);
            number_disabled = changeschartoint(snumber_disabled);
            if(number_disabled < 3 && number_disabled > 0){
                for(int i = 0;i<number_disabled;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabled > 3 || number_disabled < 0); count = 0;
    }
    else if(strcmp(status,dict[1]) == 0) /*หย่า*/{
        *discount += 60000; //personal allowance
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father or Mother = 1 Father&Mother = 2 skip = 0)\n>> ");
            scanf("%s",snumber_parents);
            number_parents = changeschartoint(snumber_parents);
            if(number_parents == 1) *discount += 30000;
            else if(number_parents == 2) *discount += 60000;
            count++;
        }while(number_parents > 2 || number_parents < 0); count = 0;
        do{
            printf("Any children?(Y/N)\n>> ");//ลดหย่อนบิดา-มารดา (คู่สมรส) Dad,Mom,DadMom
            scanf(" %c",&yn);
        }while(yn != 'y' && yn != 'n' && yn != 'Y' && yn != 'N');
        if(yn == 'y' || yn == 'Y'){
            printf("How many children do you have(child was born before 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sons);
            number_sons = changeschartoint(snumber_sons);
            if(number_sons > 0){
                for(i = 1;i <= number_sons;i++){
                    *discount += 30000;
                }
            }
            printf("How many children do you have(child was born after 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sonsafter);
            number_sonsafter = changeschartoint(snumber_sonsafter);
            if(number_sonsafter > 0){
                for(i = 1;i <= number_sonsafter;i++){
                    *discount += 60000;
                }
            }
        }
        do{
            if(count > 0) printf("Pleas fill out again.\n");
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 4 father mother sons and other.))\n>> ");
            scanf("%s",snumber_disabled);
            number_disabled = changeschartoint(snumber_disabled);
            if(number_disabled > 0 && number_disabled < 5){
                for(int i = 0;i<number_disabled;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabled > 4 || number_disabled < 0); count = 0;
    }
    else if(strcmp(status,dict[2]) == 0){
        *discount += 60000; //personal allowance
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father(Husband) or Mother(Husband) = 1 Parent(Husband)= 2 skip = 0)\n>> ");
            scanf("%s",snumber_parents);
            number_parents = changeschartoint(snumber_parents);
            if(number_parents == 1) *discount += 30000;
            else if(number_parents == 2) *discount += 60000;
            count++;
        }while(number_parents > 2 || number_parents < 0); count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father(Wife) or Mother(Wife) = 1 Parent(Wife)= 2 skip = 0)\n>> ");
            scanf("%s",snumber_parentsspouse);
            number_parentsspouse = changeschartoint(snumber_parentsspouse);
            if(number_parentsspouse == 1) *discount += 30000;
            else if(number_parentsspouse == 2) *discount += 60000;
            count++;
        }while(number_parentsspouse > 2 || number_parentsspouse < 0); count = 0;
        do{
            printf("Any children?(Y/N)\n>> ");//ลดหย่อนบิดา-มารดา (คู่สมรส) Dad,Mom,DadMom
            scanf(" %c",&yn);
        }while(yn != 'y' && yn != 'n' && yn != 'Y' && yn != 'N');
        if(yn == 'y' || yn == 'Y'){
            printf("How many children do you have(child was born before 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sons);
            number_sons = changeschartoint(snumber_sons);
            if(number_sons > 0){
                for(i = 1;i <= number_sons;i++){
                    *discount += 30000;
                }
            }
            printf("How many children do you have(child was born after 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sonsafter);
            number_sonsafter = changeschartoint(snumber_sonsafter);
            if(number_sonsafter > 0){
                for(i = 1;i <= number_sonsafter;i++){
                    *discount += 60000;
                }
            }
        }
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 4 father mother sons and other.))\n>> ");
            scanf("%s",snumber_disabled);
            number_disabled = changeschartoint(snumber_disabled);
            if(number_disabled > 0 && number_disabled < 5){
                for(int i = 0;i<number_disabled;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabled > 4 || number_disabled < 0); count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 3 spouse father(spouse) and mother(spouse).))\n>> ");
            scanf("%s",snumber_disabledspouse);
            number_disabledspouse = changeschartoint(snumber_disabledspouse);
            if(number_disabledspouse > 0 && number_disabledspouse < 4){
                for(int i = 0;i<number_disabledspouse;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabledspouse > 3 || number_disabledspouse < 0); count = 0;
    }
    else if(strcmp(status,dict[3]) == 0) /*คู่สมรสไม่มีเงินได้*/{
        *discount += 120000; //personal allowance
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father(Husband) or Mother(Husband) = 1 Parent(Husband)= 2 skip = 0)\n>> ");
            scanf("%s",snumber_parents);
            number_parents = changeschartoint(snumber_parents);
            if(number_parents == 1) *discount += 30000;
            else if(number_parents == 2) *discount += 60000;
            count++;
        }while(number_parents > 2 || number_parents < 0); count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Parent allowance(Father(Wife) or Mother(Wife) = 1 Parent(Wife)= 2 skip = 0)\n>> ");
            scanf("%s",snumber_parentsspouse);
            number_parentsspouse = changeschartoint(snumber_parentsspouse);
            if(number_parentsspouse == 1) *discount += 30000;
            else if(number_parentsspouse == 2) *discount += 60000;
            count++;
        }while(number_parentsspouse > 2 || number_parentsspouse < 0); count = 0;
        do{
            printf("Any children?(Y/N)\n>> ");//ลดหย่อนบิดา-มารดา (คู่สมรส) Dad,Mom,DadMom
            scanf(" %c",&yn);
        }while(yn != 'y' && yn != 'n' && yn != 'Y' && yn != 'N');
        if(yn == 'y' || yn == 'Y'){
            printf("How many children do you have(child was born before 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sons);
            number_sons = changeschartoint(snumber_sons);
            if(number_sons > 0){
                for(i = 1;i <= number_sons;i++){
                    *discount += 30000;
                }
            }
            printf("How many children do you have(child was born after 2561) ? (fill in numbers)\n>> ");
            scanf("%s",snumber_sonsafter);
            number_sonsafter = changeschartoint(snumber_sonsafter);
            if(number_sonsafter > 0){
                for(i = 1;i <= number_sonsafter;i++){
                    *discount += 60000;
                }
            }
        }
        do{
            if(count > 0) printf("Please fill out again.\n"); 
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 4 father mother sons and other.))\n>> ");
            scanf("%s",snumber_disabled);
            number_disabled = changeschartoint(snumber_disabled);
            if(number_disabled > 0 && number_disabled < 5){
                for(int i = 0;i<number_disabled;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabled > 4 || number_disabled < 0); count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Disabled allowance(has no income)(How many do you handler, disabled people?(maximum is 3 spouse father(spouse) and mother(spouse).))\n>> ");
            scanf("%s",snumber_disabledspouse);
            number_disabledspouse = changeschartoint(snumber_disabledspouse);
            if(number_disabledspouse > 0 && number_disabledspouse < 4){
                for(int i = 0;i<number_disabledspouse;i++){
                    *discount += 60000;
                }
            }
            count++;
        }while(number_disabledspouse > 3 || number_disabledspouse < 0); count = 0;
    }
    if(income*12 - expense(income) - *discount <= 0) data[20] = 2;
}

//รายการลดหย่อนภาษี : ประกัน อิงจากเมนู 4(ประกัน) kasikorn
void insurance(int *discount,int income){
    //int root,checkroot = 0;
    int Health_insurance,Life_insurance,count = 0,fifteen_percentof_income = (15*income)/100,Annuity_insurance_premium,prakan,interest,Parents_health_insurance;
    int *pAnnuity_insurance_premium = &Annuity_insurance_premium;
    do{
        if(count > 0)
            printf("Life insurance and Health insurance combined must not exceed 100,000 bath please fill information again.\n");
        Life_insurance = mistakenumber("Life insurance","maximum 100,000 bath",100000); //เบี้ยประกันชีวิต
        Health_insurance = mistakenumber("Health insuracne","maximum 25,000 bath",25000); //เบี้ยประกันสุขภาพ
        count++;
    }while(Life_insurance + Health_insurance > 100000);
    Parents_health_insurance = mistakenumber("Parents health insurance premium","maximum 15,000 bath",15000); //เบี้ยประกันสุขภาพบิดา,มารดา
    //system("cls");
    // <= 15% of income and boundaries of annity insurance is 200000 bath.
    prakan = mistakenumber("Social security","maximum 9,000 bath",9000); //เงินประกันสังคม
    interest = mistakenumber("interest","maximum 100,000 bath",100000); //ดอกเบี้ยที่อยู่อาศัย
    *discount += Life_insurance + Health_insurance + Parents_health_insurance + prakan + interest;
    data[0] = Life_insurance; data[1] = Health_insurance ; data[2] = Parents_health_insurance; data[3] = prakan; data[4] = interest;
    if(income*12 - expense(income) - *discount <= 0) data[20] = 2;
}


void fund(int *discount,int income){
    int pvd,annuity,gpf,nsf,teacher_fund,total_fund,ssf,rmf,count = 0,countt = 0;
    int annual_income = (15*12*income)/100; //รายได้ทั้งปี
    char spvd[255],sannuity[255],sgpf[255],snsf[255],steacher_fund[255],sssf[255],srmf[255];
    do{
        if(countt > 0) printf("Total must not exceed 500,000 bath please fill out again.\n");
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("PVD(less than 15%% of salary and less than 500,000 bath)\n>> ");
            scanf("%s",spvd);
            pvd = changeschartoint(spvd);
            count++;
        }while(pvd > (15*income)/100 || pvd > 500000);
        count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Annuity insurance premium(less than 15%% of annual income and less than 200,000 bath)\n>> "); //annual income is yearly income.
            scanf("%s",sannuity);
            annuity = changeschartoint(sannuity);
            count++;
        }while(annuity > annual_income || annuity > 200000);
        count = 0;
        do{
            //system("cls");
            if(count > 0) printf("Please fill out again.\n");
            printf("Government Pension Fund(less than 15%% of annual income)\n>> "); //กองทุนบำเหน็จบำนาญข้าราชการ(GPF)
            scanf("%s",sgpf);
            gpf = changeschartoint(sgpf);
            count++;
        }while(gpf > annual_income);
        count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("National Savings Fund(less than 13200 bath)\n>> "); //กองทุนออมเงินแห่งชาติ
            scanf("%s",snsf);
            nsf = changeschartoint(snsf);
            count++;
        }while(nsf > 13200);
        count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("Private teacher aid fund contribution(less than 15%% of annual income\n>> "); //กองทุนครูเอกชน
            scanf("%s",steacher_fund);
            teacher_fund = changeschartoint(steacher_fund);
            count++;
        }while(teacher_fund > annual_income);
        count = 0;
        do{
            //system("cls");
            if(count > 0) printf("Please fill out again.\n");
            printf("SSF(less than 30%% of annual income)\n>> ");
            scanf("%s",sssf);
            ssf = changeschartoint(sssf);
            count++;
        }while(ssf > (30*12*income)/100);
        count = 0;
        do{
            if(count > 0) printf("Please fill out again.\n");
            printf("RMF(less than 30%% of annual income)\n>> ");
            scanf("%s",srmf);
            rmf = changeschartoint(srmf);
            count++;
        }while(rmf > (30*12*income)/100);
        count = 0;
        total_fund = pvd + annuity + gpf + nsf + teacher_fund + ssf + rmf;
        data[5] = pvd; data[6] = annuity; data[7] = gpf; data[8] = nsf; data[9] = teacher_fund; data[10] = ssf; data[11] = rmf;
        countt++;
    }while(total_fund > 500000);
    *discount += total_fund;
    if(income*12 - expense(income) - *discount <= 0) data[20] = 2;
}


int netincomee(int *discount,int income){
    int netin,expense;
    expense = (50*income*12)/100;
    if(expense > 100000)
        expense = 100000;
    netin = income*12 - expense - *discount;
    if(data[20] == 2) netin = 0;
    data[12] = expense;
    return netin;
}


int expense(int income){
    int expense;
    expense = (50*income*12)/100;
    if(expense > 100000)
        expense = 100000;
    return expense;
}

//เอามาลดหย่อนด้วยเงินบริจาคอีกที
int donate(int netincome,int income,int *discount)/*ฟังก์ชันเงินบริจาค*/{
    //int root,checkroot = 0;
    int scholarships,donation,total_donation,count = 0;
    char sscholarships[255],sdonation[255];
    if(data[20] == 2){
        printf("exceeded the amount that can be donated.");
        total_donation = 0;
        data[13] = 0; data[14] = 0;
    }
    else{
    do{
        if(count > 0) printf("Please fill out again.\n");
        printf("Donation for Scholarships\n>> ");  //bug//
        scanf("%s",sscholarships);
        scholarships = changeschartoint(sscholarships);
        count++;
    }while(scholarships > (10*netincome)/100); count = 0;
    do{
        if(count > 0) printf("Please fill out again.\n");
        printf("General Donation\n>> ");
        scanf("%s",sdonation);
        donation = changeschartoint(sdonation);
        count++;
    }while(donation > (10*netincome)/100); count = 0;
    total_donation = scholarships + donation;
    if(data[20] != 2){
        data[13] = scholarships; data[14] = donation;
    }
    }
    return total_donation;
}

//ฟังก์ชันหาอัตราภาษี
int rateoftax(int netincome,int otherincome){
    int tax,taxmao;
    if(data[20] == 2){
        tax = 0; taxmao = 0;
        data[15] = 0; data[16] = 0;
        return tax;
    }
    else if(data[20] != 2){
    if(netincome >= 1 && netincome <= 150000)
        tax = 0;
    else if(netincome >= 150001 && netincome <= 300000)
        tax = ((netincome-150000) * 5)/100;
    else if(netincome >= 300001 && netincome <= 500000)
        tax = ((netincome-300000) * 10)/100 + 7500;
    else if(netincome >= 500001 && netincome <= 750000)
        tax = ((netincome-500000) * 15)/100 + 27500;
    else if(netincome >= 750001 && netincome <= 1000000)
        tax = ((netincome-750000) * 20)/100 + 65000;
    else if(netincome >= 1000001 && netincome  <= 2000000)
        tax = ((netincome-1000000) * 25)/100 + 115000;
    else if(netincome >= 2000001 && netincome <= 5000000)
        tax = ((netincome-2000000) * 30)/100 + 365000;
    else if(netincome > 5000000)
        tax = ((netincome-5000000) * 35)/100 + 1265000;
    taxmao = otherincome*5/1000;
    data[15] = tax; data[16] = taxmao;
    if(tax >= taxmao)
        return tax;
    else
        return taxmao;
    }
}

void inputname(char name[]){
    int check,count = 0;
    while(check != 1){
        check = 0;
        if(count > 0) printf("Error Please fill out again.\n");
        count = 0;
        do{
            if(count > 0) printf("Error Please fill out again.\n");
            printf("Input your name(Firstname Lastname)\n>> ");
            scanf(" %[^\n]s",name);
            count++;
        }while(strlen(name) == 1); count = 0;
        for(int i = 1;i<=strlen(name)-2;i++){
            if(name[i] == ' ') check++;
        }
        if(name[0] == ' ') check = 0;
        if(name[strlen(name)-1] == ' ') check = 0;
        count++;
    }
}