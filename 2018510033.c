#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
struct Records
    {
        char name[64];
        char surname[32];
        char gender;
        char email[32];
        char phone_number[16];
        char address[32];
        char level_of_education[8];
        unsigned int income_level;
        unsigned int expenditure;
        char currency_unit[16];
        char current_mood[32];
        float height;
        unsigned int weight;
    };  
//created struct
typedef struct Records my_datas;
int ChangeEndianness(unsigned int value){
    unsigned int a0,a1,a2,a3,res;
    a0=(value & 0x000000ff)<<24u;
    a1=(value & 0x0000ff00)<<8u;
    a2=(value & 0x00ff0000)>>8u;
    a3=(value & 0xff000000)>>24u;
    res=a0 | a1 | a2 |a3;
    return res;
}
//created Big-endian to Little-endian and Little-endian to Big-endian function
int main(int argc,char *argv[])
{
    FILE *rf = fopen(argv[1],"r");
    FILE *utf16r= fopen("records.dat","rb, ccs=UTF-16LE");
    FILE *wf = fopen(argv[2],"w");
    FILE *rxsd= fopen("records.xsd","w");
    //file are opened
    my_datas rItem;
    my_datas utf16_item;
    my_datas tags;
    //struct element created
    int data_id=0;
    fprintf(wf,"<Records>\n");
    fread(&tags,sizeof(my_datas),1,rf) > 0 && strcmp(tags.name,"")!=0;//tags struct filled
    fread(&utf16_item,sizeof(my_datas),1,utf16r);//for miss first step 
    while((fread(&rItem,sizeof(my_datas),1,rf) > 0 && strcmp(rItem.name,"")!=0)){
        fread(&utf16_item,sizeof(my_datas),1,utf16r);//read utf16 surname to write    
        
            fprintf(wf,"\t<row id=\"%d\">\n",data_id);
            fprintf(wf,"\t\t<%s>%s</%s>\n",tags.name,utf16_item.name,tags.name);
            fprintf(wf,"\t\t<%s>%s</%s>\n",tags.surname,rItem.surname,tags.surname);
            fprintf(wf,"\t\t<gender>%c</gender>\n",rItem.gender);
            fprintf(wf,"\t\t<%s>%s</%s>\n",tags.email,rItem.email,tags.email);
            fprintf(wf,"\t\t<%s>%s</%s>\n",tags.phone_number,rItem.phone_number,tags.phone_number);
            fprintf(wf,"\t\t<%s>%s</%s>\n",tags.address,rItem.address,tags.address);
            fprintf(wf,"\t\t<level_of_education>%s</level_of_education>\n",rItem.level_of_education);
            unsigned int endian_data;
            endian_data = ChangeEndianness(rItem.income_level);
            fprintf(wf,"\t\t<income_level bigEnd=\"%u\">%u</income_level>\n",endian_data,rItem.income_level);
            unsigned int endian_data2;
            endian_data2 = ChangeEndianness(rItem.expenditure);
            fprintf(wf,"\t\t<expenditure bigEnd=\"%u\">%u</expenditure>\n",rItem.expenditure,endian_data2);
            fprintf(wf,"\t\t<currency_unit>%s</currency_unit>\n",rItem.currency_unit);
            fprintf(wf,"\t\t<current_mood>%s</current_mood>\n",rItem.current_mood);
            fprintf(wf,"\t\t<height>%.2f</height>\n",rItem.height);
            fprintf(wf,"\t\t<weight>%u</weight>\n",rItem.weight);
            fprintf(wf,"\t</row>\n",data_id);
            data_id++;
    }
    //closing files
    fprintf(wf,"</Records>");
    fclose (rf);
    fclose (utf16r);
    fclose (wf);
    //createing xsd
    fprintf(rxsd,"<<xs:schema attributeFormDefault=\"unqualified\" elementFormDefault=\"qualified\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n");
    fprintf(rxsd,"\t<xs:element name=\"Records\">\n");
    fprintf(rxsd,"\t\t<xs:complexType>\n");
    fprintf(rxsd,"\t\t\t<xs:sequence>\n");
    fprintf(rxsd,"\t\t\t\t<xs:element masOccurs=\"unbounded\" name=\"row\">\n");
    fprintf(rxsd,"\t\t\t\t\t<xs:complexType>\n");
    fprintf(rxsd,"\t\t\t\t\t\t<xs:sequence>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"name\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"surname\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"gender\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"email\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"phone_number\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"address\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"level_of_education\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"income_level\">\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t<xs:complexType>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t<xs:simpleContent>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t<xs:extension base=\"xs:unsignedShort\">\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t\t<xs:attribute name=\"bigEnd\" type=\"xs:unsignedInt\" use=\"required\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t</xs:extension>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t</xs:simpleContent>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t</xs:complexType>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t</xs:element>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"expenditure\">\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t<xs:expenditure>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t<xs:simpleContent>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t<xs:extension base=\"xs:unsignedShort\">\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t\t<xs:attribute name=\"bigEnd\" type=\"xs:unsignedInt\" use=\"required\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t\t</xs:extension>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t\t</xs:simpleContent>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t\t</xs:complexType>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t</xs:element>\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"currency_unit\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"current_mood\" type=\"xs:string\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"weight\" type=\"xs:decimal\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t\t<xs:element name=\"height\" type=\"xs:unsignedByte\" />\n");
    fprintf(rxsd,"\t\t\t\t\t\t</xs:sequence>\n");
    fprintf(rxsd,"\t\t\t\t\t\t<xs:attribute name=\"id\" type=\"xs:unsignedByte\" use=\"required\" />\n");
    fprintf(rxsd,"\t\t\t\t\t</xs:complexType>\n");
    fprintf(rxsd,"\t\t\t\t</xs:element>\n");
    fprintf(rxsd,"\t\t\t</xs:sequence>\n");
    fprintf(rxsd,"\t\t</xs:complextype>\n");
    fprintf(rxsd,"\t</xs:element>\n");
    fprintf(rxsd,"</xs:schema>\n");
    return 0;
}