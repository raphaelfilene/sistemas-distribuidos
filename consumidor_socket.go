package main

import (
    "os"
    "bufio"
	"strconv"
	"math"
    "net"
	"log"
    "strings"
)

const (
	PORTA = 1000
    SIM = "É PRIMO!"
    NAO = "NÃO É PRIMO!"
)

func SocketConsumidor(porta int) {
	listen, erro := net.Listen("tcp4",":"+strconv.Itoa(porta))
	defer listen.Close()
	if erro != nil {
		log.Fatalf("Ouvindo de %d. Falha: %s", porta, erro)
		os.Exit(1)
	}
	log.Printf("Ouvindo de %d", porta)

	for {
		conexao, erro := listen.Accept()
		if erro != nil {
			log.Fatalln(erro)
			continue
		} 
        handler(conexao)
	}
}

func handler(conexao net.Conn) {
	defer conexao.Close()

	var (
		buf = make([]byte, 1024)
		ler   = bufio.NewReader(conexao)
		escrever   = bufio.NewWriter(conexao)
    )
    
    var dados string

    n, erro := ler.Read(buf)
    dados = string(buf[:n])

    switch erro {
        case nil:
            if checar_fluxo(dados) {}

        default:
            log.Fatalf("Falha no recebimento de dados: %s", erro)
            return
    }
        
    
    i, erro2 := strconv.ParseInt(dados,10, 32)
    if(erro2!= nil){}
    Ni := int(i)

    resultado := verificar_primo(Ni)

	escrever.Write([]byte(resultado))
	escrever.Flush()
    log.Printf("Recebeu: %s, Enviou: %s", dados, resultado)
  
    
    if(int(i) == -1){
        log.Printf("End: Consumidor")
        os.Exit(2)
    }
}

func verificar_primo(valor int) string {
    max := int(math.Floor(float64(valor)/2));
    for i := 2; i <= max; i++ { if valor%i == 0 {return NAO} }
    if(valor>1){return SIM} else{return NAO}
}

func checar_fluxo(dados string) (over bool) {
	over = strings.HasSuffix(dados, "\r\n\r\n")
	return
}

func main() {
    porta := PORTA
    log.Printf("Start: Consumidor")
	SocketConsumidor(porta)
}
