import unittest
import os
import kgekit.io
import kgedata

class EntityNumberIndexerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        triple_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'fixtures', 'triple.txt')
        t, _ = kgekit.io.read_triples(triple_filename, triple_order="hrt", delimiter=' ')
        cls._indexer = kgedata.EntityNumberIndexer(t, "hrt")

    def test_attrs(self):
        self.assertEqual(self._indexer.entityIdMap(), {'/m/entity1': 0, '/m/entity2': 1, '/m/entity3': 2, '/m/entity4': 3})
        self.assertEqual(self._indexer.relationIdMap(), {'/country': 1, '/produced_by': 0, '/produced_in': 2})
        self.assertEqual(self._indexer.indexes(), [kgedata.TripleIndex(0, 0, 1), kgedata.TripleIndex(1, 1, 2), kgedata.TripleIndex(0, 2, 2), kgedata.TripleIndex(2, 2, 3), kgedata.TripleIndex(0, 1, 3)])
        self.assertEqual(self._indexer.entities(), ['/m/entity1', '/m/entity2', '/m/entity3', '/m/entity4'])
        self.assertEqual(self._indexer.relations(), ['/produced_by', '/country', '/produced_in'])

if __name__ == '__main__':
    unittest.main()
