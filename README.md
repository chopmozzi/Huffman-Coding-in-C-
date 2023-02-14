# Huffman_Coding_in_CPP
## Introduction
- c++로 작성된 Huffman_Coding입니다.
- Encoder와 Decoder 두개로 나누어져 동작합니다.
- Huffman Coding에 대한 내용과 세부설계과정은 해당 링크에서 확인할 수 있습니다.
- [Huffman_Coding(notion)](https://abalone-fahrenheit-80e.notion.site/Huffman-Coding-47050af0444245cd8980b7273e9eedfe)
---
## How to run?
- Visual Studio Community 2019`(Project Build)`로 작성되었습니다.
- Encoding할 텍스트 파일의 이름은 `'input.txt'` HuffmanCodingEncoder 폴더 내에 존재해야합니다.
- Decoding을 하기 위해서 Encoding과정에서 생성된 `'huffman_table.hbs'`, `'huffman_code.hbs'`파일이 HuffmanCodingDecoder 폴더 내에 존재해야합니다(옮겨야 합니다).
- Visual Studio Community 프로젝트 내에서 빌드하여 Encoding과 Decoding과정을 진행합니다.
- HuffmanCodingDecoder 폴더 내에 생성된 output.txt파일을 통해 결과를 확인할 수 있습니다.
---
## Example
![input.txt 예시](/image/1.PNG)
- Huffman coindg을 이용하여 압축할 input.txt 예시

![encoding실행 예시1](/image/2.PNG)
- encoding을 통해 글자의 빈도 수를 확인할 수 있다.

![encoding실행 예시2](/image/3.PNG)
- Huffman Tree로 생성 된 이진 코드(codeword)를 확인할 수 있다.

![Huffman table 예시](/image/5.PNG)
- ASCII code - 빈도 수 - codeword의 순으로 Huffman table이 작성됨.

![Huffman code 예시](/image/4.PNG)
- codeword를 기반으로 encoding을 한 결과가 huffman_code.hbs파일로 생성된다.

![decoding실행 예시](/image/6.PNG)
- Huffman table을 바탕으로 Huffman tree를 재구성하고 encoding된 Huffman code를 decoding한다.

![output.txt 예시](/image/7.PNG)
- input.txt와 동일하게 결과가 나오는 것을 확인할 수 있다.
