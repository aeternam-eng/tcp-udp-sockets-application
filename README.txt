O software contido neste arquivo consiste em duas aplicações de comunicação por sockets:

tcp-socket-* -> par cliente-servidor que utiliza TCP como protocolo de comunicação
udp-socket-* -> par cliente-servidor que utiliza UDP como protocolo de comunicação

Ambas as aplicações são desenvolvidas com o sistema LINUX em mente, utilizando o sys/socket.h
ao invés da api de sockets do windows.
Há também alguns comentários dentro do código para facilitar o entendimento.

Tanto para a aplicação UDP quanto a TCP, ambas consistem em um sistema de "echo", onde:
	1 - O cliente se conecta
	2 - O cliente envia uma string
	3 - O servidor recebe a string
	4 - O servidor envia a mesma string de volta ao cliente
	5 - O cliente se desconecta

Não conseguimos adicionar threading e multiclient na aplicação TCP dentro do tempo, 
ocorreram algumas exceções que não conseguimos encontrar solução, 
mas é planejada a implementação.

É possível compilar e executar todos os arquivos da seguinte forma:
Compilação:	g++ main.cpp -o main
Execução: 	./main
	
No caso dos clientes, é possível passar um argumento na execução, que permite enviar uma
string específica:
	./main "String a ser enviada"

Caso a string enviada seja "stop", o servidor será encerrado.

