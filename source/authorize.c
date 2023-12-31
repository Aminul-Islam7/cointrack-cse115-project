#include "../headers/cointrack.h"

void inputPass(char *pass, int maxLen) {
    int i = 0;
    char c;
    while (1) {
        c = getch();

        if (c == '\r' || c == '\n') { // Enter key
            pass[i] = '\0';
            break;
        } else if (c == '\b' || c == 127) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the last character
            }
        } else if (i < maxLen - 1) { // Check if there is space in the buffer
            pass[i++] = c;
            printf("*");
        }
    }
}


void regUser()
{
    while (1)
    {
        header("REGISTER");
        
        printf("\t\tYour Name: \n");
        printf("\t\tPhone Number: \n");
        printf("\t\t5-digit PIN: \n");
        
        hLine();
        nl;

        command("\t   <-  ");
        printf("Back");
        nl;
        command("\tENTER  ");
        printf("Type");


        char command = getch();
        
        if (command == 13)
        {
            showCursor();
            textYellow();

            User user;
            
            user.id = genUserId();
            user.active = 1;

            moveCursor(30, 10);
            fflush(stdin);
            fgets(user.name, 50, stdin);

            moveCursor(30, 11);
            fflush(stdin);
            fgets(user.phone, 14, stdin);
            
            moveCursor(30, 12);
            fflush(stdin);
            inputPass(user.pin, 6);

            hideCursor();
            
            if (strlen(user.name) < 2)
            {
                alert("\n\t\tName can't be blank!", 2);
                continue;
            }

            if (duplicatePhone(user.phone))
            {
                alert("\n\t\tPhone number already exists! Try logging in.", 2);
                continue;
            }

            if (strlen(user.phone) != 12)
            {
                alert("\n\t\tPhone number must have 11 digits!", 2);
                continue;
            }

            if (strlen(user.pin) != 5)
            {
                alert("\n\t\tPIN must have 5 digits!", 2);
                continue;
            }
            
            success("\n\t\tYou're Registered and Logged in!", 1.5);

            FILE *file = appendFile("users.dat");

            fwrite(&user, sizeof(User), 1, file);

            userCount++;
            activeUserCount++;
            // incrementUserCount();
            loggedIn = 1;
            curUserId = user.id;
            fclose(file);
            return;
        }
        else if (command == -32)
        {
            char command = getch();
            if (command == 75)
            {
                return;
            }
            else
            {
                alert("Invalid key!", 1);
            }
        }
        else
        {
            alert("Invalid key!", 1);
        }
    }
}

void login()
{
    while (1)
    {
        header("LOGIN");

        printf("\t\tPhone: \n");
        printf("\t\tPIN: \n");
        
        hLine();
        nl;

        command("\t   <-  ");
        printf("Back");
        nl;
        command("\tENTER  ");
        printf("Type");


        char command = getch();
        
        if (command == 13)
        {
            showCursor();
            textYellow();

            char phone[12], pin[6];
            moveCursor(23, 10);
            fflush(stdin);
            scanf("%s", &phone);

            moveCursor(23, 11);
            fflush(stdin);
            inputPass(pin, 6);
            
            hideCursor();

            FILE *file = readFile("users.dat");

            User user[userCount];
            fread(user, sizeof(User), userCount, file);

            for (int i = 0; i < userCount; i++)
            {
                stripNewLine(user[i].phone);
                stripNewLine(user[i].pin);
                if (strcmp(user[i].phone, phone) == 0 && strcmp(user[i].pin, pin) == 0 && user[i].active)
                {
                    success("\n\t\tLogged in!", 1);
                    loggedIn = 1;
                    curUserId = user[i].id;
                    FILE *loginFile = writeFile("login_status.dat");
                    fwrite(&loggedIn, sizeof(int), 1, loginFile);
                    fwrite(&adminLoggedIn, sizeof(int), 1, loginFile);
                    fwrite(&curUserId, sizeof(int), 1, loginFile);
                    fclose(loginFile);
                    return;
                }
            }
            if (!loggedIn)
            {
                hideCursor();
                alert("\n\t\tInvalid credentials!", 2);
            }
            
            fclose(file);

        }
        else if (command == -32)
        {
            char command = getch();
            if (command == 75)
            {
                return;
            }
            else
            {
                alert("Invalid key!", 1);
            }
        }
        else
        {
            alert("Invalid key!", 1);
        }
    }
}

void adminLogin()
{
    while (1)
    {
        header("ADMIN LOGIN");

        char email[100], password[100];
        printf("\t\tEmail: \n");
        printf("\t\tPassword: \n");
        
        hLine();
        nl;

        command("\t   <-  ");
        printf("Back");
        nl;
        command("\tENTER  ");
        printf("Type");


        char command = getch();
        
        if (command == 13)
        {
            showCursor();
            textYellow();

            moveCursor(26, 10);
            fflush(stdin);
            scanf("%s", &email);

            moveCursor(26, 11);
            fflush(stdin);
            inputPass(password, 100);
            
            
            hideCursor();

            FILE *file = readFile("admins.dat");
            

            Admin admin[adminCount];
            fread(admin, sizeof(Admin), adminCount, file);
            for (int i = 0; i < adminCount; i++)
            {
                stripNewLine(admin[i].email);
                stripNewLine(admin[i].password);
                if (strcmp(admin[i].email, email) == 0 && strcmp(admin[i].password, password) == 0)
                {
                    success("\n\t\tLogged in!", 1);
                    adminLoggedIn = 1;
                    curUserId = admin[i].id;
                    FILE *loginFile = writeFile("login_status.dat");
                    fwrite(&loggedIn, sizeof(int), 1, loginFile);
                    fwrite(&adminLoggedIn, sizeof(int), 1, loginFile);
                    fwrite(&curUserId, sizeof(int), 1, loginFile);
                    fclose(loginFile);
                    return;
                }
            }
            if (!adminLoggedIn)
            {
                hideCursor();
                alert("\n\t\tInvalid credentials!", 2);
            }
            
            fclose(file);

        }
        else if (command == -32)
        {
            char command = getch();
            if (command == 75)
            {
                return;
            }
            else
            {
                alert("Invalid key!", 1);
            }
        }
        else
        {
            alert("Invalid key!", 1);
        }
    }
}


void logout()
{
    while (1)
    {
        header("LOGOUT");
        
        textYellow();
        printf("\tAre you sure you want to logout?\n");
        textWhite();

        hLine();
        nl;

        command("\tENTER  ");
        printf("Yes");
        nl;
        command("\t   <-  ");
        printf("No");

        char command = getch();

        switch (command)
        {
        case 13:
            loggedIn = 0;
            adminLoggedIn = 0;
            curUserId = 0;

            FILE *loginFile = writeFile("login_status.dat");
            fwrite(&loggedIn, sizeof(int), 1, loginFile);
            fwrite(&adminLoggedIn, sizeof(int), 1, loginFile);
            fwrite(&curUserId, sizeof(int), 1, loginFile);
            fclose(loginFile);
            
            header("GOODBYE");

            textYellow();
            printf("\tLogged out!", 0);
            textWhite();
            nl;
            hLine();
            Sleep(1500);
            return;
        case -32:
            command = getch();
            if (command == 75)
            {
                return;
            }
            else
            {
                alert("Invalid key!", 1);
            }
            return;
        default:
            alert("Invalid key!", 1);
            break;
        }

        nl;
        nl;
    }

}