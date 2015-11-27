/*
Test run this program by copying this code to main.cpp of the IDE and building it.
....
A simple program that implements an instance of S-DES Decryption.
A user inputs ciphertext, and 2 keys, (all 8bits). The code will enforce 8 bit input and validate binary numbers.
The program will return the plaintext by performing the standard DES functions as defined below.
The bits of input need to be entered one by one, so for example to enter the binary number 11011110..
You enter: 1 (Press enter) 1 (Press enter) 0 (Press enter) 1.....etc.
.....
Test data:
K1=10011110, K2=11001001, C=10110111, P=10101010
K1=10100010, K2=00011011, C=00001010, P=00100100
.....
by: Shayan Memari (18096125)
*/

#include <iostream>
#include <iomanip>
using namespace std;


//Function to perform IP function and return pointer to the result.
int * initPerm(int input[8]){
    static int ip[8] = {2,6,3,1,4,8,5,7};
    static int ip_out[8];
    for (int i=0; i<8;i++){
        int pos = ip[i];
        ip_out[i] = input[pos-1];
    }
    return ip_out;
}

//Performs inverse permutation and returns pointer to it.
int * initPermInv(int input[8]){
    static int ip1[8] = {4,1,3,5,7,2,8,6};
    static int ip_out2[8];
    for (int i=0; i<8;i++){
        int pos = ip1[i];
        ip_out2[i] = input[pos-1];
    }
    return ip_out2;
}

//Function to return the Expansion/Permutation function.
int * exPermut (int input[4]){
    static int ep[8] = {4,1,2,3,2,3,4,1};
    static int ep_out[8];
    for (int i=0; i<8;i++){
        int pos = ep[i];
        ep_out[i] = input[pos-1];
    }
    return ep_out;

}

//Returns XOR of input with a Key.
int * xorKey (int input [8], int Key[8]){
    static int xor_out[8];
    for (int i=0; i<8;i++){
        xor_out[i] = input[i] ^ Key[i];
    }
    return xor_out;
}

//Quick function to convert a 2 bit binary to Decimal
//For finding out the row and column in SBox.
int binaryToDec(int bitOne, int bitTwo){
    if (bitOne==0 && bitTwo==0){
        return 0;
    }
    if (bitOne==0 && bitTwo==1){
        return 1;
    }
    if (bitOne==1 && bitTwo==0){
        return 2;
    }
    if (bitOne==1 && bitTwo==1){
        return 3;
    }
}

//Converts from decimal(0-3) to Binary.
int * decToBinary(int num){
    static int bin[2];
    if (num == 0 ){
        bin[0]=0;
        bin[1]=0;
    }
    if (num == 1 ){
        bin[0]=0;
        bin[1]=1;
    }
    if (num == 2 ){
        bin[0]=1;
        bin[1]=0;
    }
    if (num == 3 ){
        bin[0]=1;
        bin[1]=1;
    }

    return bin;
}

//Takes in a 4 bit input and returns 2 bit output.
int * sBox0(int input[4]){
    static int s0[4][4] = {1,0,3,2,
                           3,2,1,0,
                           0,2,1,3,
                           3,1,3,2};
    static int sbox0_out[2];
    int row = binaryToDec(input[0], input[3]);
    int col = binaryToDec(input[1], input[2]);
    int num = s0[row][col];
    int * bin = decToBinary(num);
    for (int i=0;i<2;++i){
        sbox0_out[i] = *(bin+i);
    }

    return sbox0_out;
}

//Takes in a 4 bit input and returns 2 bit output.
int * sBox1(int input[4]){
    static int s1[4][4] = {0,1,2,3,
                            2,0,1,3,
                            3,0,1,0,
                            2,1,0,3};

    static int sbox1_out[2];
    int row = binaryToDec(input[0], input[3]);
    int col = binaryToDec(input[1], input[2]);
    int num = s1[row][col];
    int * bin2 = decToBinary(num);
    for (int i=0;i<2;++i){
        sbox1_out[i] = *(bin2+i);
    }

    return sbox1_out;
}

//Performs P4 Function.
int * p4(int input[4]){
    static int p4[4] = {2,4,3,1};
    static int p4_out[4];
    for (int i=0; i<4;i++){
        int pos = p4[i];
        p4_out[i] = input[pos-1];
    }
    return p4_out;

}

//Performs XOR between the two inputs and returns pointer to the array.
int * xorResult(int input[4], int input2[4]){
    static int xor_out2[4];
    for (int i=0; i<4;i++){
        xor_out2[i] = input[i] ^ input2[i];
    }
    return xor_out2;
}



int main()
{
    int cipherText[8];  //The 8 bit ciphertext.
    int firstKey[8];    //the 8 bit Key 1.
    int secondKey[8];   //The 8 bit key 2.
    int plainText[8];   //Resulting plaintext calculated.
    int input_bit;      //Used for validating each bit from user input.
    string input;       //Holds the binary number.
    bool valid=true;    //For Validating input

    do
    {
        cout << "Enter CIPHERTEXT (8-bits): \n";
        cin >> input;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '1') {
                cipherText[i] = 1;
                valid=true;
            } else if (input[i] == '0') {
                cipherText[i] = 0;
                valid=true;
            } else {
                valid=false;
                cout << "\n[ERROR]Incorrect input, 0s and 1s only.\n";
                break;
            }
        }
        if (input.size() != 8) {
            cout << "[ERROR]8-bit binary number allowed only...\n";
            valid=false;
        }
    } while (!valid);

    do
    {
        cout << "Enter the FIRST KEY (8-bits): \n";
        cin >> input;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '1') {
                firstKey[i] = 1;
                valid=true;
            } else if (input[i] == '0') {
                firstKey[i] = 0;
                valid=true;
            } else {
                valid=false;
                cout << "\nIncorrect input, 0s and 1s only.\n";
                break;
            }
        }
        if (input.size() != 8) {
            cout << "8-bit binary number allowed only...\n";
            valid=false;
        }
    } while (!valid);

    do
    {
        cout << "Enter the SECOND KEY (8-bits): \n";
        cin >> input;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '1') {
                secondKey[i] = 1;
                valid=true;
            } else if (input[i] == '0') {
                secondKey[i] = 0;
                valid=true;
            } else {
                valid=false;
                cout << "\n[ERROR]Incorrect input, 0s and 1s only.\n";
                break;
            }
        }
        if (input.size() != 8) {
            cout << "[ERROR]8 bits binary number allowed only...\n";
            valid=false;
        }
    } while (!valid);


    //print out all the inputs to confirm.
    cout << "\nThe CIPHERTEXT is: ";
    cout << setw(4);
    for (int i=0;i<8;++i){
        cout << cipherText[i];
    }
    cout << "\nYour FIRST KEY is: ";
    cout << setw(4);
    for (int i=0;i<8;++i){
        cout << firstKey[i];
    }
    cout << "\nThe SECOND KEY is: ";
    cout << setw(4);
    for (int i=0;i<8;++i){
        cout << secondKey[i];
    }
    //Perform IP on Ciphertext
    int *p = initPerm(cipherText);
    int left[4];    //Left 4 bits of IP
    int right[4];   //Right 4 bits of IP
    for (int i=0;i<8;++i){
            if (i < 4){
                left[i] = *(p+i);
            }
            else
            {
                right[i-4] = *(p+i);
            }
    }
    //Perform E/P on the right 4 bits.
    int * exp = exPermut(right);
    int result_EP[8];
    for (int i=0;i<8;++i){
        result_EP[i] = *(exp+i);
    }
    //Perform XOR with K2
    int * Xor2_pt = xorKey(result_EP, secondKey);
    int leftx[4];   //Left 4 bits of the XOR with K2
    int rightx[4];  //Right 4 bits of the XOR with K2
    for (int i=0;i<8;++i){
            if (i < 4){
                leftx[i] = *(Xor2_pt+i);
            }
            else
            {
                rightx[i-4] = *(Xor2_pt+i);
            }
    }
    //SBOX0 with leftside.
    int sbox_Output[4]; //The 4 bits combined output.
    int * sbox0_pt = sBox0(leftx);
    for (int i=0;i<2;++i){
        sbox_Output[i] = *(sbox0_pt + i);
    }
    //SBOX1 with rightside.
    int * sbox1_pt = sBox1(rightx);
    for (int i=0;i<2;++i){
        sbox_Output[i+2] = *(sbox1_pt + i);
    }
    //Perform P4 on it
    int * p4_pt = p4(sbox_Output);
    int p4_output[4];
    for (int i=0;i<4;++i){
        p4_output[i] = *(p4_pt+i);
    }
    //Perform fk with Left side of IP.
    int fk_left[4];
    int * Xor_pt = xorResult(left, p4_output);
    for (int i=0;i<4;++i){
        fk_left[i] = *(Xor_pt+i);
    }
    int first_out[8];   //The 8 bit result of fK: (L, R) = (L XOR f(R, key), R)...
    for (int i=0;i<4;++i){
        first_out[i] = fk_left[i];
    }
    for (int i=0;i<4;++i){
        first_out[i+4] = right[i];
    }
    //Switch left 4 bits with the right 4 bits.
    int swappedResult[8];
    for (int i=0;i<4;++i){
        swappedResult[i] = first_out[i+4];
    }
    for (int i=0;i<4;++i){
        swappedResult[i+4] = first_out[i];
    }
    //---Perform second stage of Decryption.
    //Separated the switched result into two 4 bits left and right.
    int swappedLeft[4];
    int swappedRight[4];
    for (int i=0;i<8;++i){
            if (i < 4){
                swappedLeft[i] = swappedResult[i];
            }
            else
            {
                swappedRight[i-4] = swappedResult[i];
            }
    }
    //Perform E/P on the right 4 bits.
    int * exp2 = exPermut(swappedRight);
    int result_EP2[8];
    for (int i=0;i<8;++i){
        result_EP2[i] = *(exp2+i);
    }
    //Perform XOR with K1
    int * Xor1_pt = xorKey(result_EP2, firstKey);
    int leftx1[4];   //Left 4 bits of the XOR with K1
    int rightx1[4];  //Right 4 bits of the XOR with K1
    for (int i=0;i<8;++i){
            if (i < 4){
                leftx1[i] = *(Xor1_pt+i);
            }
            else
            {
                rightx1[i-4] = *(Xor1_pt+i);
            }
    }
    //SBOX0 with leftside.
    int sbox_Output2[4]; //The 4 bits combined output.
    int * sbox0_pt2 = sBox0(leftx1);
    for (int i=0;i<2;++i){
        sbox_Output2[i] = *(sbox0_pt2 + i);
    }
    //SBOX1 with rightside.
    int * sbox1_pt2 = sBox1(rightx1);
    for (int i=0;i<2;++i){
        sbox_Output2[i+2] = *(sbox1_pt2 + i);
    }
    //Perform P4 on it
    int * p4_pt2 = p4(sbox_Output2);
    int p4_output2[4];
    for (int i=0;i<4;++i){
        p4_output2[i] = *(p4_pt2+i);
    }
    //Perform fk with Left side of the switched bits.
    int fk_left2[4];
    int * Xor_pt2 = xorResult(swappedLeft, p4_output2);
    for (int i=0;i<4;++i){
        fk_left2[i] = *(Xor_pt2+i);
    }
    int second_out[8];   //The 8 bit result of fK: (L, R) = (L XOR f(R, key), R)...
    for (int i=0;i<4;++i){
        second_out[i] = fk_left2[i];
    }
    for (int i=0;i<4;++i){
        second_out[i+4] = swappedRight[i];
    }

    //Perform IP-1 on Ciphertext
    int *p2 = initPermInv(second_out);
    cout << "\nYour PLAINTEXT is: ";
    cout << setw(4);
    for (int i=0;i<8;++i){
        plainText[i] = *(p2+i);
        cout << plainText[i];
    }
    //To Keep the screen Open.
    int stop;
    cin >> stop;
}


