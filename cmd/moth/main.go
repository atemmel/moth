package main

import (
	"bufio"
	"fmt"
	"mothcli/pkg/compile"
	"os"
	"os/exec"
	"time"

	"github.com/atemmel/tflags"
)

var (
	home string
	dll string
)

func mpath(file string) string {
	const prefix = ".local/share/moth/"
	return home + prefix + file
}

func defaultArgs() compile.Args {
	return compile.Args{
		IncludeDirs: []string{
			"src/",
			mpath("include"),
		},
		LibDir: mpath("bin"),
		OutDir: "build",
		OutName: "shared.dll",
		Release: false,
		Target: compile.Linux,
	}
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
	args := defaultArgs()
	fmt.Println("Building lib...")
	err := compile.Lib(args)
	if err != nil {
		panic(err)
	}
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
