#!/usr/bin/python3

from sys import argv
from shutil import copy
from string import Template
from argparse import ArgumentParser
from os import makedirs, path, readlink, system

BASE_DIR = path.dirname(readlink(argv[0]))

def create_dir(name):
    if not path.exists(name):
        makedirs(name)

def read_file(name):
    with open(name, "r") as f:
        return f.read()

def install_test_framework(name):
    test_include = '${CMAKE_SOURCE_DIR}/src '
    print('Adding {} test framework'.format(name))
    if name == 'yatf':
        system('git submodule add https://github.com/Mrokkk/yatf.git test/yatf')
        test_include += 'yatf/include'
    elif name == 'catch':
        system('git submodule add https://github.com/philsquared/Catch.git test/Catch')
        test_include += 'Catch/single_include'
    test_cmake = Template(read_file(BASE_DIR + '/templates/test.CMakeLists.txt')).substitute(test_include=test_include)
    with open('test/CMakeLists.txt', 'w') as f:
        f.write(test_cmake)
    copy(BASE_DIR + '/templates/{}.main.cpp'.format(name), 'test/main.cpp')

def init_project(args):
    print('Initializing {}'.format(args.name))
    system('git init')
    create_dir('src')
    create_dir('test')
    with open('CMakeLists.txt', 'w') as f:
        f.write('project({} CXX)\n'.format(args.name))
        f.write(read_file(BASE_DIR + '/templates/CMakeLists.txt'))
    copy(BASE_DIR + '/templates/src.CMakeLists.txt', 'src/CMakeLists.txt')
    copy(BASE_DIR + '/templates/src.main.cpp', 'src/main.cpp')
    system('git submodule add https://github.com/Mrokkk/cmake-utils.git test/cmake-utils')
    install_test_framework(args.test_framework)

def add_init_command(subparsers):
    parser_share = subparsers.add_parser('init')
    parser_share.add_argument('name')
    parser_share.add_argument('-t', '--test-framework', choices=['yatf', 'catch', 'none'], default='yatf')
    parser_share.set_defaults(func=init_project)

def run_project(args):
    create_dir('build')
    project_name = None
    with open('CMakeLists.txt', 'r') as f:
        line = f.readline()
        project_name = line[line.index('(') + 1:line.rindex(')')].split()[0]
    system('cd build; cmake .. && make {} && ./src/{}'.format(project_name, project_name))

def add_run_command(subparsers):
    parser_share = subparsers.add_parser('run')
    parser_share.set_defaults(func=run_project)

def test_project(args):
    create_dir('build')
    system('cd build; cmake .. && make tests-run -j$(nproc)')

def add_test_command(subparsers):
    parser_share = subparsers.add_parser('test')
    parser_share.set_defaults(func=test_project)

def main():
    parser = ArgumentParser()
    subparsers = parser.add_subparsers(help='sub-command help')
    add_init_command(subparsers)
    add_run_command(subparsers)
    add_test_command(subparsers)
    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()

# vim: filetype=python
