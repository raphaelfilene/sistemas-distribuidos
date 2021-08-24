package main

import (
    "log"
	"net"
    "time"
	"strconv"
    "strings"
    "flag"
    "math/rand"
)

const (
    MINIMO = 1
    MAXIMO = 100
    QTD_ITERACOES = 10
    IP = "127.0.0.1"
    PORTA = 1000
)

func SocketProdutor(ip string, porta int) {

    rand.Seed(time.Now().UnixNano())
    Ni := MINIMO+rand.Intn(MAXIMO-MINIMO)

    for i:=0;i<QTD_ITERACOES;i++{
        Ni=Ni+MINIMO+rand.Intn(MAXIMO-MINIMO)
        if(i==QTD_ITERACOES-1){Ni=-1}

        str_numero:=strconv.Itoa(Ni)
        endereco:=strings.Join([]string{ip, strconv.Itoa(porta)},":")
        
        conexao,erro := net.Dial("tcp",endereco)

        defer conexao.Close()

        if erro != nil {log.Fatalln(erro)}
        
        conexao.Write([]byte(str_numero))
        
        resultado := make([]byte, 1024)
        n, _ := conexao.Read(resultado)
        log.Printf("Enviou: %s - Recebeu %s", str_numero,resultado[:n])
    }
}

func main() {
	var (
		ip   = IP
		porta = PORTA
    )
    log.Printf("Start: Produtor")
    flag.Parse()
    SocketProdutor(ip,porta)
    log.Printf("End: Produtor")
}