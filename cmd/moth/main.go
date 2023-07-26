package main

import (
	"bufio"
	"github.com/atemmel/tflags"
	"os"
	"os/exec"
	"time"
)

var (
	home string
	dll string
)

func mpath(file string) string {
	const suffix = ".local/share/moth/"
	return home + suffix + file
}

func init() {
	home = os.Getenv("HOME") + "/"
	tflags.String(&dll, &tflags.Meta{
		Long: "dll",
		Short: "d",
		Help: "dll to use",
	});
	tflags.Parse()
}

func main() {
	cmd := exec.Command(mpath("bin/loader"), dll)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	rawIn, err := cmd.StdinPipe()
	if err != nil {
		panic(err)
	}
	in := bufio.NewWriter(rawIn)
	err = cmd.Start()
	if err != nil {
		panic(err)
	}
	in.WriteString("register:EmptyScene\n")
	in.Flush()
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
