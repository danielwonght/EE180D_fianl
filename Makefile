all: inField_Consumer inField_Collector dataConsumer dataCollector

inField_Consumer: inField_Consumer.c extractStrides.c extractFeatures.c featureDivider.c feature_filter.c conversion.c train_neural_net.c test_network.c
	gcc -o inField_Consumer inField_Consumer.c extractStrides.c extractFeatures.c featureDivider.c feature_filter.c conversion.c train_neural_net.c test_network.c -lm -lfann

inField_Collector: inField_Collector.c data_collection.c
	gcc -o inField_Collector inField_Collector.c data_collection.c LSM9DS0.c -lm -lmraa

dataConsumer: dataConsumer.c extractStrides.c extractFeatures.c decision_maker.c
	gcc -o dataConsumer dataConsumer.c extractStrides.c extractFeatures.c decision_maker.c -lm -lmraa -lfann

dataCollector: dataCollector.c LSM9DS0.c
	gcc -o dataCollector dataCollector.c LSM9DS0.c -lm -lmraa

clean:
	rm -rf inField_Consumer ~*
	rm -rf inField_Collector ~*
	rm -rf dataConsumer ~*
	rm -rf dataCollector ~*
