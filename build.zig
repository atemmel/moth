const std = @import("std");

const Lib = std.Build.LibExeObjStep;
const Target = std.zig.CrossTarget;
const Optimize = std.builtin.OptimizeMode;

const common_flags = &[_][]const u8{
    "-std=c++20",
    "-Wall",
    "-Wextra",
    "-Wshadow",
    "-Wswitch-enum",
};

const vendor_path = "vendor/";
const lib_path = vendor_path ++ "lib";
const include_path = vendor_path ++ "include";

fn buildMothLib(b: *std.Build, target: Target, optimize: Optimize) void {
    const lib = b.addSharedLibrary(.{
        .name = "libmoth",
        .target = target,
        .optimize = optimize,
    });
    lib.addCSourceFiles(&.{
        "moth/moth.cpp",
        "moth/dynlib.cpp",
    }, common_flags);
    lib.defineCMacro("_WIN32", null);
    lib.linkLibC();
    lib.linkLibCpp();
    lib.addLibraryPath(lib_path);
    lib.addIncludePath(include_path);
    lib.addObjectFile(lib_path ++ "/SDL2.lib");
    b.installArtifact(lib);
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    buildMothLib(b, target, optimize);
    const exe = b.addExecutable(.{
        .name = "moth",
        .target = target,
        .optimize = optimize,
        .linkage = .dynamic,
    });
    exe.addIncludePath("loader");
    exe.addCSourceFiles(&.{
        "loader/main.cpp",
    }, common_flags);
    exe.defineCMacro("_WIN32", null);
    exe.linkLibC();
    exe.linkLibCpp();
    exe.addObjectFile("zig-out/lib/libmoth.lib");
    exe.addObjectFile(lib_path ++ "/SDL2.lib");
    exe.addObjectFile(lib_path ++ "/SDL2main.lib");
    b.installArtifact(exe);
}
