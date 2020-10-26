#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
//#include <locale.h>

struct dirent *de;
char search[100];
char presearch[100];
char command[100];
char mdname[100];
char gpath[100];
char local[100][100];
char readstr[1000];
DIR *entry;
DIR *preentry;
char control;
int maxlines, maxcols;

int main()
{
	initscr();
	use_legacy_coding(2);
	cbreak();
	noecho();
	start_color();
	init_color(COLOR_RED,0,200,250);  //marine
	init_color(COLOR_CYAN,0,800,800);
	init_color(COLOR_BLACK,0,80,80);    //extra mute cyan
	init_color(COLOR_BLUE,0,300,300);    //mute cyan
	init_color(COLOR_GREEN,1000,650,0); //orange
	init_pair(1,COLOR_RED,COLOR_CYAN);
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	bkgd(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));
	curs_set(0);
	getmaxyx(stdscr, maxlines, maxcols);
	WINDOW *first_win;
	first_win = newwin(maxlines-4,maxcols/3-1,3,1);
	WINDOW *second_win;
	second_win = newwin(maxlines-4,maxcols/3-1,3,maxcols/3);
	WINDOW *third_win;
	third_win = newwin(maxlines-4,maxcols/3-1,3,maxcols/3*2);
	WINDOW *input_win;
	input_win = newwin(1,maxcols-2,1,1);
	FILE *openfile;
	FILE *helpfile;
	wattron(input_win,COLOR_PAIR(2));
	wattron(first_win,COLOR_PAIR(4));
	wattron(second_win,COLOR_PAIR(3));
	attron(A_REVERSE);
	box(stdscr,0,0);
	mvaddstr(0,maxcols/2-5,"joj-manager");
	mvaddstr(maxlines-1, 2, "I-input path C-change dir H-help Q-quit");
	mvhline(2,1,0,maxcols-2);
	mvaddstr(2,1,"UPPER LEVEL");
	mvaddstr(2,maxcols/3,"CURRENT LEVEL");
	mvaddstr(2,maxcols/3*2,"LOG");
	attroff(A_REVERSE);
	move(1,2);
	refresh();
	while(true){
	control = getch();
	switch(control)
	{
		case('i'):
		case('I'):
			echo();
			curs_set(1);
			werase(input_win);
			werase(first_win);
			werase(second_win);
			werase(third_win);
			wprintw(input_win,"-input-  /");
			wscanw(input_win,"%s", search);
			strcpy(gpath,search);
			strcpy(search,"/");
			strcat(search,gpath);
			werase(input_win);
			wprintw(input_win, search);
			wrefresh(input_win);
			noecho();
			curs_set(0);
			entry = opendir(search);
			strcpy(presearch, search);
			strcat(presearch,"/..");
			preentry = opendir(presearch);
			if(entry==NULL)
			{
				wattron(input_win,A_REVERSE);
				wprintw(first_win,"no such dir\n");
				wattroff(input_win,A_REVERSE);
			}
			else
			{
				int counter = 0;
				while((de=readdir(entry))!=NULL)
				{
					strcpy(local[counter], de->d_name);
					if(strcmp(local[counter],".")!=0&&strcmp(local[counter],"..")!=0)
						wprintw(second_win,"%s\n", local[counter]);
					counter++;
				}
				while((de=readdir(preentry))!=NULL)
					wprintw(first_win,"%s\n", de->d_name);
			}
			wprintw(third_win, "%s\n%s",search,presearch);
			closedir(entry);
			closedir(preentry);
			wrefresh(first_win);
			wrefresh(second_win);
			wrefresh(third_win);
			break;
		case('Q'):
		case('q'):
			werase(input_win);
			wattron(input_win,A_REVERSE);
			wprintw(input_win, "Are you sure you want to quit? y/n");
			wattroff(input_win,A_REVERSE);
			wrefresh(input_win);
			control = getch();
			if(control == 'y' || control == 'Y')
			{
					
				endwin();
				return 0;
			}
			else
				werase(input_win);
				wprintw(input_win, search);
				wrefresh(input_win);
				break;
		case('n'):
		case('N'):
			echo();
			curs_set(1);
			werase(input_win);
			werase(first_win);
			werase(second_win);
			werase(third_win);
			wprintw(input_win,"-name new dir-  ");
			wscanw(input_win,"%s", mdname);
			werase(input_win);
			strcpy(command, "mkdir ");
			strcat(command, search);
			strcat(command, "/");
			strcat(command, mdname);
			system(command);
			wprintw(input_win, "Directory %s has been created", mdname);
			wrefresh(second_win);
			wrefresh(input_win);
			noecho();
			curs_set(0);
			break;
		case('R'):
		case('r'):
			echo();
			curs_set(1);
			werase(input_win);
			werase(first_win);
			werase(second_win);
			werase(third_win);
			wprintw(input_win,"-remove-  ");
			wscanw(input_win,"%s", mdname);
			werase(input_win);
			wattron(input_win,A_REVERSE);
			wprintw(input_win, "Are you sure you want to remove %s? y/n", mdname);
			wattroff(input_win,A_REVERSE);
			wrefresh(input_win);
			control = getch();
			if(control == 'y' || control == 'Y')
			{
				strcpy(command, "rm -R ");
				strcat(command, search);
				strcat(command, "/");
				strcat(command, mdname);
				system(command);
				werase(input_win);
				wprintw(input_win, "%s has been removed", mdname);
				wrefresh(second_win);
				wrefresh(input_win);
				noecho();
				curs_set(0);
				break;
			}
			else
				werase(input_win);
				wprintw(input_win, search);
				wrefresh(input_win);
				break;
		case('c'):
		case('C'):
			echo();
			curs_set(1);
			werase(input_win);
			wprintw(input_win,"-change dir-  ");
			wscanw(input_win,"%s", mdname);
			curs_set(0);
			noecho();
			werase(input_win);
			strcpy(gpath,search);
			strcat(gpath,"/");
			strcat(gpath,mdname);
			strcpy(presearch, gpath);
			strcat(presearch,"/..");
			entry = opendir(gpath);
			preentry = opendir(presearch);
			if(entry==NULL)
			{
				werase(input_win);
				wattron(input_win,A_REVERSE);
				wprintw(input_win,"no such dir\n");
				wattroff(input_win,A_REVERSE);
				wrefresh(input_win);
			}
			else
				{
					strcpy(search,gpath);
					werase(input_win);
					werase(first_win);
					werase(second_win);
					werase(third_win);
					wprintw(input_win, gpath);
					wrefresh(input_win);
					int counter = 0;
					while((de=readdir(entry))!=NULL)
					{
						strcpy(local[counter], de->d_name);
						if(strcmp(local[counter],".")!=0&&strcmp(local[counter],"..")!=0)
							wprintw(second_win,"%s\n", local[counter]);
						counter++;
					}
					while((de=readdir(preentry))!=NULL)
						wprintw(first_win,"%s\n", de->d_name);
				}
				wprintw(third_win, "%s\n%s",search,presearch);
				wrefresh(first_win);
				wrefresh(second_win);
				wrefresh(third_win);
			break;
		case('o'):
		case('O'):
			echo();
			curs_set(1);
			werase(input_win);
			werase(first_win);
			werase(second_win);
			werase(third_win);
			wprintw(input_win,"-open file?-  ");
			noecho();
			wscanw(input_win,"%s", mdname);
			werase(input_win);
			strcpy(gpath, search);
			strcat(gpath, "/");
			strcat(gpath, mdname);
			openfile=fopen(gpath, "rt");
			if(openfile!=NULL)
			{
				while(fgets(readstr, 1000, openfile)!=NULL)
					wprintw(third_win,"%s",readstr);
				werase(input_win);
				wprintw(input_win,"%s",gpath);
				wrefresh(input_win);
				wrefresh(third_win);
					
			}
			else
			{
				werase(input_win);
				wattron(input_win,A_REVERSE);
				wprintw(input_win,"no such file");
				wattroff(input_win,A_REVERSE);
				wrefresh(input_win);
			}
			break;
		case('p'):
		case('P'):
			echo();
			curs_set(1);
			werase(input_win);
			werase(first_win);
			werase(second_win);
			werase(third_win);
			wprintw(input_win,"-copy which file-  ");
			wscanw(input_win,"%s", mdname);
			werase(input_win);
			strcpy(gpath, search);
			strcat(gpath, "/");
			strcat(gpath, mdname);
			openfile=fopen(gpath, "rt");
			if(openfile!=NULL)
			{
				strcpy(command, "cp ");
				strcat(command, search);
				strcat(command, "/");
				strcat(command, mdname);
				strcat(command, " ");
				wrefresh(input_win);
				wprintw(input_win,"-copy to-  /");
				wscanw(input_win,"%s", mdname);
				werase(input_win);
				char temp[100];
				strcpy(temp,"/");
				strcat(temp,mdname);
				entry = opendir(temp);
				if(entry==NULL)
				{
					werase(input_win);
					wattron(input_win,A_REVERSE);
					wprintw(input_win,"no such dir\n");
					wattroff(input_win,A_REVERSE);
					wrefresh(input_win);
				}
				else
				{
					strcat(command, temp);
					wprintw(input_win,"copied");
					system(command);
					wrefresh(input_win);
				}
			}
			else
			{
				werase(input_win);
				wattron(input_win,A_REVERSE);
				wprintw(input_win,"no such file");
				wattroff(input_win,A_REVERSE);
				wrefresh(input_win);
			}
			break;
		case('h'):
		case('H'):
			werase(third_win);
			helpfile=fopen("help","rt");
			while(fgets(readstr, 1000, helpfile)!=NULL)
				wprintw(third_win,"%s",readstr);
			wrefresh(third_win);
			wrefresh(third_win);
			break;
		default:
			//any key
			break;
			
	}}
	endwin();
	return 0;
}


