// Copyright (c) 2021 Giannis Gonidelis

#include <algorithm> 

#include <range/v3/all.hpp>
#include <nanobench.h>

#include "hpx/hpx.hpp"
// #include "hpx/hpx_main.hpp"
#include "hpx/init.hpp"
#include "hpx/algorithm.hpp"
#include "hpx/modules/program_options.hpp"

unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);

void reverse(int n)
{
    std::vector<int> vec(n);
    std::fill(
        std::begin(vec), std::end(vec), gen() % 1000);

    ankerl::nanobench::Bench().run("reverse() then foreach()", [&] {
            ankerl::nanobench::Bench().epochs(3);
        hpx::ranges::reverse(hpx::execution::par, vec);
        hpx::ranges::for_each(hpx::execution::par, vec, [](auto n){return 0;});
        // ankerl::nanobench::doNotOptimizeAway(vec);
    });

    ankerl::nanobench::Bench().run("foreach(reverse view)", [&] {
        hpx::ranges::for_each(hpx::execution::par, vec | ranges::views::reverse, 
            [](auto n){return 0;});
        // ankerl::nanobench::doNotOptimizeAway(vec);
    });
}

void transform(int n)
{
    std::vector<int> vec(n);
    std::fill(
        std::begin(vec), std::end(vec), gen() % 1000);


    ankerl::nanobench::Bench()
        .output(nullptr)
        .run("transform() then foreach()", [&] {
            ankerl::nanobench::Bench().epochs(3);
            hpx::ranges::transform(hpx::execution::par, vec, vec.begin(), [](auto i){return i * 2;});
            hpx::ranges::for_each(hpx::execution::par, vec, [](auto n){return 0;});
            // ankerl::nanobench::doNotOptimizeAway(vec);
        })
        .render(ankerl::nanobench::templates::csv(), std::cout);

    ankerl::nanobench::Bench()
        .output(nullptr)
        .run("foreach(transform view)", [&] {
            ankerl::nanobench::Bench().epochs(3);
            hpx::ranges::for_each(hpx::execution::par, vec 
                | ranges::views::transform(
                    [](int i){return i * 2;}), [](auto n){return 0;});
            // ankerl::nanobench::doNotOptimizeAway(vec);
        })
        .render(ankerl::nanobench::templates::csv(), std::cout);
}

int hpx_main(hpx::program_options::variables_map& vm) {
	std::string algo = vm["algo"].as<std::string>();
	int n = vm["n"].as<int>();
	// std::string algorithm_filename = vm["output"].as<std::string>();


    if(algo == "transform")
    {
        transform(n);
    }
    else if(algo == "reverse" )
    {
        reverse(n);
    }

	return hpx::finalize();
}

int main(int argc, char* argv[])
{
	using namespace hpx::program_options;
	options_description desc_commandline("usage" HPX_APPLICATION_STRING " [options]");

	desc_commandline.add_options()
        ("algo", hpx::program_options::value<std::string>(), "algorithm for benchmarking")
		("n", hpx::program_options::value<int>()->default_value(1'000'000), "number of elements");
		// ("output", hpx::program_options::value<std::string>()->default_value("unique_replace_if_transform.csv"), "algorithm output filename");

	// Initialize and run HPX
	hpx::init_params init_args;
	init_args.desc_cmdline = desc_commandline;

	return hpx::init(argc, argv, init_args);
}