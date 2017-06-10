#!/usr/bin/python3

import os
import sys
import shutil
import argparse

BASE_DIR = os.path.dirname(sys.argv[0])

def create_dir(name):
    if not os.path.exists(name):
        os.makedirs(name)

def read_file(name):
    with open(name, "r") as f:
        return f.read()

def init_project(args):
    print('Initializing {}'.format(args.name))
    os.system('git init')
    create_dir('src')
    create_dir('test')
    with open('CMakeLists.txt', 'w') as f:
        f.write('project({} CXX)\n'.format(args.name))
        f.write(read_file(BASE_DIR + '/templates/CMakeLists.txt'))
    shutil.copy(BASE_DIR + '/templates/src.CMakeLists.txt', 'src/CMakeLists.txt')
    shutil.copy(BASE_DIR + '/templates/src.main.cpp', 'src/main.cpp')
    shutil.copy(BASE_DIR + '/templates/test.main.cpp', 'test/main.cpp')
    shutil.copy(BASE_DIR + '/templates/test.CMakeLists.txt', 'test/CMakeLists.txt')
    os.system('git submodule add https://github.com/Mrokkk/cmake-utils.git test/cmake-utils')

def add_init_command(subparsers):
    parser_share = subparsers.add_parser('init')
    parser_share.add_argument('name')
    parser_share.set_defaults(func=init_project)

def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(help='sub-command help')
    add_init_command(subparsers)
    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()

# vim: filetype=python
