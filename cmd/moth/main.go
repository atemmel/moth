package main

import (
	"bufio"
	"os"
	"os/exec"
	"time"
)

func main() {

	//cmd := exec.Command("./loader.exe")
	cmd := exec.Command("/home/temmel/.local/share/moth/loader")
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

type CompileTarget int

const (
	Linux CompileTarget = 0
	Windows = iota
)

type CompileArgs struct {
	Compiler string
	Target CompileTarget
	ImportLib bool
	OutName string
	OutDir string
}

func compile(args CompileArgs) error {
	return nil
}
