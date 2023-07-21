package main

import (
	"bufio"
	"os"
	"os/exec"
	"time"
)

func main() {
	cmd := exec.Command("./loader.exe")
	cmd.Stdout = os.Stdout
	rawIn, err := cmd.StdinPipe()
	if err != nil {
		panic(err)
	}
	in := bufio.NewWriter(rawIn)
	err = cmd.Start()
	if err != nil {
		panic(err)
	}
	time.Sleep(time.Second * 2)
	in.WriteString("reload\n")
	in.Flush()
	err = cmd.Wait()
	if err != nil {
		panic(err)
	}
}
