# FileSystem EXT3  ( Sistema de arquivo ext3)

Trabalho de Engenharia de Computação UFSC : Arquitetura de Sistemas Operacionas.

Os testes são realizados da seguinte forma:

Você deve ler o comando do stdin (teclado). O comando informa o arquivo que simula o sistema de arquivos EXT3.
Você deve executar o comando solicitado sobre o sistema de arquivos informado.
Você deve executar a função printSha256() passando como parâmetro o nome do arquivo que simula o sistema de arquivos.

Os comandos são os seguintes:
init -> cria um novo sistema de arquivos EXT3
Exemplo: criar um novo sistema de arquivos com 10 blocos de 5 bytes e 2 inodes. Salvar o arquivo em fs.bin

```
init fs.bin 5 10 2
```

addFile -> adiciona um novo arquivo a um sistema de arquivos
Exemplo: criar o arquivo /teste.txt com o conteúdo abcd no sistemas de arquivo fs.bin. O conteúdo não deve ter espaços em branco

```
add fs.bin /teste.txt abcd
```
addDir -> adiciona um diretório ao sistema de arquivos
Exemplo: adicionar o diretório /etc no sistema de arquivos fs.bin

```
addDir fs.bin /etc
```
