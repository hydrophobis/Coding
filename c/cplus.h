
bool findstr(char* str, char* search){
    int counter = 0;
    for(int i = 0; i < str.length(); i++;){
        if(str[i] == search[counter]){
            counter += 1;
        } else {
            counter = 0;
        }
        
        if(i == str.length() - 1){
            return false;
        }
    }
    return true;
}
    