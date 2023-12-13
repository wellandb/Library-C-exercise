/* complete this file */
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "library.h"
#include <sys/types.h>  // needed for open
#include <sys/stat.h>   // needed for open
#include <fcntl.h>      // needed for open
#include <unistd.h>     // needed for read/write

// Document
// getters/setters
void Document::updateTitle(const char *newTitle){
    size_t sizeT = strlen(newTitle);
    _title = new char[sizeT - 1];
    strcpy(_title, newTitle);
}
void Document::updateYear(int newYear){
    _year = newYear;
}
void Document::updateQuantity(int newQuantity){
    _quantity = newQuantity;
}
char *Document::getTitle(){
    return _title;
}
int Document::getYear(){
    return _year;
}
int Document::getQuantity(){
    return _quantity;
}
// virtual 
void Document::print(){
    this->print();
}
document_type Document::getDocType(){
    return this->getDocType();
}
// borrow and return
int Document::borrowDoc(){
    int currentQuant = this->getQuantity();
    if(currentQuant > 0){
        updateQuantity(currentQuant - 1);
        return 0;
    }else{
        printf("No copies in library\n");
        return -1;
    }
}
void Document::returnDoc(){
    updateQuantity(getQuantity() + 1);
}

// Novel
Novel::Novel(const char *title, const char *author, int year, int quantity){
    updateTitle(title);
    updateAuthor(author);
    updateYear(year);
    updateQuantity(quantity);
}
Novel::~Novel(){
    delete [] _author;
    delete [] _title;
}

// getters/setters
void Novel::updateAuthor(const char *newAuthor){
    _author = new char[strlen(newAuthor) - 1];
    strcpy(_author, newAuthor);
}

char *Novel::getAuthor(){
    return _author;
}
// virtual
document_type Novel::getDocType(){
    return DOC_NOVEL;
}
void Novel::print(){
    printf("Novel, title: %s, author: %s, year: %d, quantity: %d\n", getTitle(), getAuthor(), getYear(), getQuantity());
}

// Comic
Comic::Comic(const char *title, const char *author, int issue, int year, int quantity){
    updateTitle(title);
    updateAuthor(author);
    updateIssue(issue);
    updateYear(year);
    updateQuantity(quantity);
}
Comic::~Comic(){
    delete [] _author;
    delete [] _title;
}
// getters/setters
void Comic::updateAuthor(const char *newAuthor){
    _author = new char[strlen(newAuthor) - 1];
    strcpy(_author, newAuthor);
}
char *Comic::getAuthor(){
    return _author;
}
void Comic::updateIssue(int newIssue){ 
    _issue = newIssue;
}
int Comic::getIssue(){
    return _issue;
}
// virtual
document_type Comic::getDocType(){
    return DOC_COMIC;
}
void Comic::print(){
    printf("Comic, title: %s, author: %s, issue: %d, year: %d, quantity: %d\n", getTitle(), getAuthor(), getIssue(), getYear(), getQuantity());
}

// Magazine
Magazine::Magazine(const char *title, int issue, int year, int quantity){
    updateTitle(title);
    updateIssue(issue);
    updateYear(year);
    updateQuantity(quantity);
}
Magazine::~Magazine(){
    delete [] _title;
}
// getters/setters
void Magazine::updateIssue(int newIssue){ 
    _issue = newIssue;
}
int Magazine::getIssue(){
    return _issue;
}
// virtual
document_type Magazine::getDocType(){
    return DOC_MAGAZINE;
}
void Magazine::print(){
    printf("Magazine, title: %s, issue: %d, year: %d, quantity: %d\n", getTitle(), getIssue(), getYear(), getQuantity());
}

// Library
Library::Library(){
    vector<Document *> _docs;
    _docs.clear();
}
void Library::print(){
    if(_docs.size() != 0){
        for(int count = 0; count < _docs.size(); count++){
            _docs[count]->print();
        }
    }
}
int Library::dumpCSV(const char *filename){
    int file = open(filename, O_CREAT | O_RDWR, S_IRWXU);
    int bufferSize = _docs.size() * 100; // estimated size of buffer
    for(int count = 0; count < _docs.size(); count++){
        char *buffer = new char[bufferSize];
        document_type docType = _docs[count]->getDocType();
        if(docType == DOC_NOVEL){
            Novel * d = dynamic_cast<Novel*>(_docs[count]);
            sprintf(buffer, "novel,%s,%s,,%d,%d\n", d->getTitle(),d->getAuthor(),d->getYear(), d->getQuantity());
            if(write(file, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error when writing to CSV file\n");
                delete [] buffer;
                close(file);
                return -1;
            }
        }
        if(docType == DOC_MAGAZINE){
            Magazine * d= dynamic_cast<Magazine*>(_docs[count]);
            sprintf(buffer, "magazine,%s,,%d,%d,%d\n", d->getTitle(),d->getIssue(),d->getYear(), d->getQuantity());
            if(write(file, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error when writing to CSV file\n");
                delete [] buffer;
                close(file);
                return -1;
            }
        }
        if(docType == DOC_COMIC){
            Comic * d= dynamic_cast<Comic*>(_docs[count]);
            sprintf(buffer, "comic,%s,%s,%d,%d,%d\n", d->getTitle(),d->getAuthor(),d->getIssue(),d->getYear(), d->getQuantity());
            if(write(file, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error when writing to CSV file\n");
                delete [] buffer;
                close(file);
                return -1;
            }
        }
        delete [] buffer;
    }
    close(file);
    return 0;
}
Document *Library::searchDocument(const char *title){
    if(_docs.size() > 1){
        for (int count = 0; count < _docs.size(); count++){
            if (strcmp(_docs[count]->getTitle(),title)==0){
                return _docs[count];
            }
        }
    } else if(_docs.size() == 1){
        if(strcmp(_docs[0]->getTitle(),title)==0){
                return _docs[0];
            }
    }
    printf("There are no documents titled %s\n",title);
    return NULL;
}
// Add/delete a document to/from the library
int Library::addDocument(Document *d){
    if (searchDocument(d->getTitle()) == NULL){
        _docs.push_back(d);
        return 0;
    }
    printf("Unable to add document %s to library\n", d->getTitle());
    return -1;
}
int Library::delDocument(const char *title){
    Document *d = searchDocument(title);
    if (d != NULL){
        for(int count = 0; count< _docs.size(); count++){
            if (d == _docs[count]){
                _docs.erase(_docs.begin() + count);
            }
        }
        return 0;
    }
    return -1;
}
// Count the number of document of a given type
int Library::countDocumentOfType(document_type t){
    int counter = 0;
    for (int count = 0; count < _docs.size(); count++){
        if (t ==_docs[count]->getDocType()){
            counter += 1;
        }
    }
    printf("There are %d documents of that type\n", counter);
    return counter;
}
// borrow and return
int Library::borrowDoc(const char *title){
    Document *d = searchDocument(title);
    if (d!=NULL){
        return d->borrowDoc();
    }
    return -1;
}
int Library::returnDoc(const char *title){
    Document *d = searchDocument(title);
    if(d!=NULL){
        d->returnDoc();
        return 0;
    }
    return -1;
}