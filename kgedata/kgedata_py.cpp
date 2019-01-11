#include <random>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "kgedata.h"
#include "entity_number_indexer.h"
#include "lcwa_no_throw_sampler.h"
#include "corruptors.h"
#include "ranker.h"
#include "triple_expander.h"

namespace py = pybind11;

/*
 * The naming convention of this file is to help mix with Python code.
 */
PYBIND11_MODULE(kgedata, m) {
#include "kgedata_type.cpp"

    m.def("get_triple_index", &kgedata::get_triple_index, "get triple index from a line",
          py::arg("line"),
          py::arg("order"),
          py::arg("delimiter"),
          py::arg("skip_checking_order") = false);
    m.def("get_triple", &kgedata::get_triple, "get triple from a line",
          py::arg("line"),
          py::arg("order"),
          py::arg("delimiter"),
          py::arg("skip_checking_order") = false);

    m.def("expand_triple_batch", &kgedata::expand_triple_batch, "expands triple batch",
          py::arg("batch"),
          py::arg("num_entity"),
          py::arg("num_relation"),
          py::arg("expand_entity"),
          py::arg("expand_relation"));

    py::class_<kgedata::EntityNumberIndexer>(m, "EntityNumberIndexer", "index from triples and translation between index and lists")
        .def(py::init<const py::list&, const std::string&>())
        .def("entityIdMap", &kgedata::EntityNumberIndexer::entityIdMap)
        .def("relationIdMap", &kgedata::EntityNumberIndexer::relationIdMap)
        .def("indexes", &kgedata::EntityNumberIndexer::indexes, "gets indexes from triples")
        .def("entities", &kgedata::EntityNumberIndexer::entities, "gets entity list")
        .def("relations", &kgedata::EntityNumberIndexer::relations, "gets relation list")
        .def("getEntityFromId", &kgedata::EntityNumberIndexer::getEntityFromId, "gets the entity name from id")
        .def("getRelationFromId", &kgedata::EntityNumberIndexer::getRelationFromId, "gets the relation name from id")
        .def("getIdFromEntity", &kgedata::EntityNumberIndexer::getIdFromEntity, "gets the entity id from name")
        .def("getIdFromRelation", &kgedata::EntityNumberIndexer::getIdFromRelation, "gets the entity name from id");

    py::enum_<kgedata::LCWANoThrowSampler::Strategy>(m, "LCWANoThrowSamplerStrategy", py::arithmetic())
        .value("Hash", kgedata::LCWANoThrowSampler::Strategy::Hash)
        .value("Offset", kgedata::LCWANoThrowSampler::Strategy::Offset);

    py::class_<kgedata::LCWANoThrowSampler>(m, "LCWANoThrowSampler")
        .def(py::init<const IndexArray&, int64_t, int64_t, int16_t, int16_t>())
        .def(py::init<const IndexArray&, int64_t, int64_t, int16_t, int16_t, int64_t>())
        .def(py::init<const IndexArray&, int64_t, int64_t, int16_t, int16_t, int64_t, kgedata::LCWANoThrowSampler::Strategy>())
        .def("numNegativeSamples", &kgedata::LCWANoThrowSampler::numNegativeSamples, "gets the number of negative samples")
        .def("sample", &kgedata::LCWANoThrowSampler::sample, py::arg("arr").noconvert(), py::arg("corrupt_head_list").noconvert(), py::arg("batch").noconvert(), "samples current batch");

    py::class_<kgedata::BernoulliCorruptor>(m, "BernoulliCorruptor", "generates the bernoulli distribution of samples")
        .def(py::init<const IndexArray&, int32_t>())
        .def(py::init<const IndexArray&, int32_t, int64_t>())
        .def("get_probability_relation", &kgedata::BernoulliCorruptor::get_probability_relation, "gets the probability for certain relation")
        .def("make_random_choice", &kgedata::BernoulliCorruptor::make_random_choice, "gets the choice for given batch item");
    py::class_<kgedata::UniformCorruptor>(m, "UniformCorruptor", "generates the uniform distribution of samples")
        .def(py::init<>())
        .def(py::init<int64_t>())
        .def("make_random_choice", &kgedata::UniformCorruptor::make_random_choice, "gets the choice for given batch item");

    py::class_<kgedata::Ranker>(m, "Ranker", "ranks the prediction")
        .def(py::init<const py::list&, const py::list&, const py::list&>())
        .def("rankHead", &kgedata::Ranker::rankHead, py::arg("arr").noconvert(), py::arg("triple").noconvert(), "returns rank and filter rank of triple of interests. Used to test head prediction.")
        .def("rankTail", &kgedata::Ranker::rankTail, py::arg("arr").noconvert(), py::arg("triple").noconvert(), "returns rank and filter rank of triple of interests. Used to test tail prediction.")
        .def("rankRelation", &kgedata::Ranker::rankRelation, py::arg("arr").noconvert(), py::arg("triple").noconvert(), "returns rank and filter rank of triple of interests. Used to test relation prediction.")
        .def("exportState", &kgedata::Ranker::exportState)
        .def(py::pickle(&kgedata::ranker_pickle_getstate, &kgedata::ranker_pickle_setstate));
}

