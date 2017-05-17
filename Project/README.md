# Packagist search engine

This project attempts to create a better searchable version of
Packagist, the PHP package repository.

## Dependencies

To compile and run this program, you need:

- GCC with C++14 support
- cURL (with development headers)
- Python3
- NetworkX for Python 3

This all can be available on ISSC machines, but requires some setup. You
can `source environment_issc` to attempt to set this up automatically,
or alternatively set this up by hand.

## Building

After correctly setting up the environment, all programs can be built by
running `make`. It is recommended to run this with the `-j` option, in
order to speed up the build.

## Usage

There are various programs in the final folder:

- `download` downloads every package meta file from packagist and
  saves it into the repository. It uses caching to speed up the download
  when refreshing the index, and has no harmful side effects when run
  multiple times.

- `indexer` creates the actual index of the packages. It can be run
  multiple times, but when you do, you should run `dedup` in order to
  clean up the repository.

- `pagerank` finally computes the pagerank of all packages for ranking
  purposes. Running this multiple times is idempotent.

All of these programs take an optional parameter: the location of the
data repository, which defaults to "data" in the current working
directory. The directory is assumed to exist and is created by the
Makefile by default. Building the index is done by running (in order):

1. download
2. indexer
3. pagerank

The resulting index is about 2.6 GiB.

## Search front-end

The search front-end is located in the "interface" folder. Composer must
be used to install its dependencies. After that, use any PHP capable
webserver to run `index.php`, or any CGI-capable webserver to run
`index2.php`.

`config.php` contains the configuration, including its relative location
to the data dir and executable dir. If those are not default, change as
needed.
