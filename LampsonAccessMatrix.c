#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERS 3
#define OBJECTS 3

char users[USERS][20] = {"Admin", "Alice", "Bob"};
char roles[USERS][20] = {"ADMIN", "USER", "USER"};

char objects[OBJECTS][20] =
{
    "File1",
    "File2",
    "File3"
};

/*
Permission Bit Values

1 = Read
2 = Write
4 = Execute

7 = RWX
*/

int matrix[USERS][OBJECTS] =
{
    {7,7,7},
    {3,1,0},
    {1,4,0}
};

int owners[OBJECTS] =
{
    1,
    1,
    2
};

void logEvent(char *msg)
{
    FILE *fp = fopen("audit.log","a");

    time_t t;
    time(&t);

    fprintf(fp,"%s : %s",msg,ctime(&t));

    fclose(fp);
}

void saveMatrix()
{
    FILE *fp = fopen("matrix.dat","w");

    for(int i=0;i<USERS;i++)
    {
        for(int j=0;j<OBJECTS;j++)
        {
            fprintf(fp,"%d ",matrix[i][j] ^ 123);
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
}

void loadMatrix()
{
    FILE *fp=fopen("matrix.dat","r");

    if(fp==NULL)
        return;

    for(int i=0;i<USERS;i++)
    {
        for(int j=0;j<OBJECTS;j++)
        {
            fscanf(fp,"%d",&matrix[i][j]);
            matrix[i][j] ^=123;
        }
    }

    fclose(fp);
}

void checkAccess(int user,int object,char op)
{
    int flag=0;

    if(strcmp(roles[user],"ADMIN")==0)
        flag=1;

    if(op=='R' && (matrix[user][object]&1))
        flag=1;

    if(op=='W' && (matrix[user][object]&2))
        flag=1;

    if(op=='X' && (matrix[user][object]&4))
        flag=1;

    if(flag)
    {
        printf("\nACCESS GRANTED\n");
        logEvent("Authorized Access");
    }
    else
    {
        printf("\nACCESS DENIED\n");
        logEvent("Unauthorized Access");
    }
}

void grantPermission()
{
    int owner,target,obj,perm;

    printf("Owner ID : ");
    scanf("%d",&owner);

    printf("Target User ID : ");
    scanf("%d",&target);

    printf("Object ID : ");
    scanf("%d",&obj);

    printf("Permission (1=R 2=W 4=X): ");
    scanf("%d",&perm);

    if(owner==owners[obj] ||
       strcmp(roles[owner],"ADMIN")==0)
    {
        matrix[target][obj]|=perm;

        printf("Permission Granted\n");

        logEvent("Permission Granted");

        saveMatrix();
    }
    else
    {
        printf("Only resource owner can grant.\n");
    }
}

void revokePermission()
{
    int owner,target,obj,perm;

    printf("Owner/Admin ID : ");
    scanf("%d",&owner);

    printf("Target User ID : ");
    scanf("%d",&target);

    printf("Object ID : ");
    scanf("%d",&obj);

    printf("Permission (1=R 2=W 4=X): ");
    scanf("%d",&perm);

    if(owner==owners[obj] ||
       strcmp(roles[owner],"ADMIN")==0)
    {
        matrix[target][obj] &= ~perm;

        printf("Permission Revoked\n");

        logEvent("Permission Revoked");

        saveMatrix();
    }
    else
    {
        printf("Permission Denied\n");
    }
}

void displayMatrix()
{
    printf("\nACCESS MATRIX\n\n");

    for(int i=0;i<USERS;i++)
    {
        printf("%s -> ",users[i]);

        for(int j=0;j<OBJECTS;j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    loadMatrix();

    int choice;

    while(1)
    {
        printf("\n===== ACCESS CONTROL SYSTEM =====\n");
        printf("1.Check Access\n");
        printf("2.Grant Permission\n");
        printf("3.Revoke Permission\n");
        printf("4.Display Matrix\n");
        printf("5.Exit\n");

        printf("Choice : ");
        scanf("%d",&choice);

        if(choice==1)
        {
            int u,o;
            char op;

            printf("User ID(0-Admin,1-Alice,2-Bob): ");
            scanf("%d",&u);

            printf("Object ID(0-File1,1-File2,2-File3): ");
            scanf("%d",&o);

            printf("Operation(R/W/X): ");
            scanf(" %c",&op);

            checkAccess(u,o,op);
        }
        else if(choice==2)
        {
            grantPermission();
        }
        else if(choice==3)
        {
            revokePermission();
        }
        else if(choice==4)
        {
            displayMatrix();
        }
        else if(choice==5)
        {
            break;
        }
    }

    return 0;
}
