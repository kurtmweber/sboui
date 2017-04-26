#pragma once

#include <string>
#include <vector>
#include <curses.h>
#include "BuildListItem.h"
#include "CategoryListItem.h"
#include "CategoryListBox.h"
#include "BuildListBox.h"

void filter_all(std::vector<std::vector<BuildListItem> > & slackbuilds,
                       std::vector<CategoryListItem> & categories,
                       WINDOW *blistboxwin, CategoryListBox & clistbox,
                       std::vector<BuildListBox> & blistboxes);
void filter_by_prop(std::vector<std::vector<BuildListItem> > & slackbuilds,
                       const std::string & propname,
                       std::vector<CategoryListItem> & categories,
                       WINDOW *blistboxwin, CategoryListBox & clistbox,
                       std::vector<BuildListBox> & blistboxes,
                       unsigned int & nfiltered);
void filter_nondeps(std::vector<std::vector<BuildListItem> > & slackbuilds,
                       std::vector<CategoryListItem> & categories,
                       WINDOW *blistboxwin, CategoryListBox & clistbox,
                       std::vector<BuildListBox> & blistboxes,
                       unsigned int & nnondeps);
void filter_search(std::vector<std::vector<BuildListItem> > & slackbuilds,
                       std::vector<CategoryListItem> & categories,
                       WINDOW *blistboxwin, CategoryListBox & clistbox,
                       std::vector<BuildListBox> & blistboxes,
                       unsigned int & nsearch, const std::string & searchterm,
                       bool case_sensitive, bool whole_word,
                       bool search_readmes);
