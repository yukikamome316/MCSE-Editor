#include <tiny_stdlib.h>

FILE *fp;
int error;
uint32_t readFile32bit(FILE *fp){
    return
        fgetc(fp)<<0x18|
        fgetc(fp)<<0x10|
        fgetc(fp)<<0x08|
        fgetc(fp);
}

void skipRead(FILE *fp,size_t pos){
    for(int i=0;i<pos;i++){
        fgetc(fp);
    }
}
bool existFile(char *filename){
    FILE *fp=fopen(filename,"r");
    if(fp==NULL){
        return false;
    }
    fclose(fp);
    return true;
}

bool createFile(char *filename){
    FILE *fp=fopen(filename,"w");
    if(fp==NULL){
        return 1;
    }
    fclose(fp);
    return 0;
}


void extractMsscmp(char *path){
    fp=fopen(path,"rb");
    if(fp==NULL){
        error=1;
        printf("Failed to open target file: %s",path);
    }
    fseek(fp,0x00000018,SEEK_SET);
    uint32_t fileTable = readFile32bit(fp);
    fseek(fp,0x00000034,SEEK_SET);
    uint32_t entryCount = readFile32bit(fp);
    uint32_t i;
    uint32_t foldNameOffset,fileNameOffset;
    uint32_t fileInfoOffset, offset, sampleRate, size;
    char filename[300], foldname[300];
    for (i = 0; i < entryCount; i++)
    {
        fseek(fp,fileTable,SEEK_SET);
        foldNameOffset = readFile32bit(fp);
        fileInfoOffset = readFile32bit(fp);

        fseek(fp,fileInfoOffset + 4,SEEK_SET);
        fileNameOffset = readFile32bit(fp) + fileInfoOffset;
        offset = readFile32bit(fp);
        skipRead(fp,8);
        sampleRate = readFile32bit(fp);
        size = readFile32bit(fp);

        fseek(fp,foldNameOffset,SEEK_SET);
        fgets(foldname,300,fp);
        fseek(fp,fileNameOffset,SEEK_SET);
        fgets(filename,300,fp);

        /* File Set an file[offset,size] dir -> foldname+"\"+filename */
        //TODO: Write extract

        fileTable += 8;
    }

}