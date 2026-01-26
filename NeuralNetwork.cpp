#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

const double initial_model_bias[] = {-0.0004266984760761261, 0.00042669830145314336};

const double initial_model_weights[] = {-0.11811771243810654, -0.11055771261453629, 0.06855766475200653, 0.08321584761142731, -0.08347561210393906, -0.07838767021894455, 0.008074570447206497, 0.026011819019913673, 0.09936264902353287, 0.11259332299232483, 0.14594249427318573, -0.06660925596952438, -0.06017632782459259, -0.1211896687746048, -0.12232860922813416, 0.09304162859916687, 0.13960155844688416, 0.026834966614842415, 0.15000203251838684, -0.05260568484663963, -0.00384838180616498, -0.0799727588891983, 0.00499866483733058, 0.1418856829404831, -0.08238502591848373, 0.13202913105487823, -0.053876329213380814, -0.03194393590092659, -0.15366525948047638, -0.15070468187332153, -0.1364509016275406, 0.04256562143564224, 0.04726783558726311, 0.06324870884418488, 0.11390653252601624, 0.00903685949742794, -0.026730842888355255, -0.023827560245990753, 0.021859724074602127, -0.1532682180404663, 0.003183932974934578, 0.01784231700003147, -0.1530141830444336, 0.08276204764842987, 0.10861870646476746, -0.0005227148067206144, -0.06642001867294312, 0.13945923745632172, -0.0987219586968422, -0.07055467367172241, -0.05470089986920357, -0.12431187182664871, -0.010953661054372787, -0.08427175134420395, -0.10095055401325226, 0.022762976586818695, 0.06578198075294495, -0.10861662030220032, 0.023014044389128685, -0.07338786125183105, 0.061932314187288284, 0.11395585536956787, -0.13732682168483734, -0.04564952850341797, -0.012083698995411396, -0.0017493385821580887, -0.05173280090093613, -0.10066059231758118, 0.11770536750555038, 0.08393111079931259, -0.015508112497627735, 0.04133535176515579, 0.10698167979717255, -0.042376160621643066, 0.052014295011758804, 0.09034064412117004, -0.06626657396554947, -0.06868262588977814, -0.14866404235363007, 0.030358856543898582, 0.10060577839612961, 0.12350056320428848, -0.016833778470754623, -0.0064192661084234715, 0.0838993638753891, 0.025136131793260574, 0.10309971868991852, -0.007472779601812363, 0.10743124037981033, -0.144083172082901, 0.05499368533492088, -0.1268589198589325, -0.15270796418190002, 0.04169844090938568, -0.10784131288528442, -0.13859295845031738, -0.13242128491401672, -0.14854446053504944, -0.12977680563926697, 0.062174420803785324, -0.14612309634685516, 0.0630558431148529, -0.12748299539089203, -0.02491888776421547, 0.06101653724908829, -0.1313793957233429, -0.08811715990304947, 0.13830658793449402, 0.09162869304418564, -0.0231650248169899, -0.08736997097730637, -0.12126211076974869, 0.016768697649240494, 0.11052189767360687, -0.0703803300857544, -0.11023721843957901, 0.0637897253036499, 0.14154160022735596, 0.0347418338060379, 0.13515111804008484, 0.12155511230230331, -0.12489961832761765, 0.02262306772172451, 0.06466992944478989, 0.15354202687740326, 0.10896158218383789, 0.07697563618421555, -0.04164418578147888, -0.0160920899361372, -0.12022415548563004, -0.11182479560375214, -0.021670512855052948, -0.06827040016651154, 0.09002815186977386, 0.13428738713264465, 0.06819016486406326, 0.1525631695985794, 0.12650276720523834, 0.12700523436069489, -0.11728338897228241, -0.10860975086688995, 0.0870208889245987, 0.020996343344449997, 0.15309123694896698, 0.07365711033344269, -0.005751458927989006, 0.0672583281993866, -0.009793230332434177, -0.15108038485050201, -0.13449369370937347, -0.06743227690458298, -0.10017052292823792, -0.13965332508087158, -0.09915744513273239, 0.04156802222132683, -0.05118454620242119, -0.125191792845726, -0.12068445235490799, 0.10670388489961624, -0.15272536873817444, 0.12341028451919556, 0.13238337635993958, -0.03628294914960861, -0.1472320258617401, 0.007108604535460472, 0.04806319251656532, 0.14768797159194946, 0.07299419492483139, -0.12866558134555817, -0.047788605093955994, 0.11556461453437805, 0.04384179413318634, 0.02889437973499298, 0.029247507452964783, 0.0902964323759079, 0.04838428273797035, -0.1376124620437622, -0.05512635409832001, 0.03975692391395569, 0.013808456249535084, 0.13633550703525543, -0.1069682314991951, 0.03437699377536774, -0.1337844580411911, -0.02797650173306465, 0.05637674778699875, 0.038827311247587204, 0.12845635414123535, 0.06155693531036377, 0.005881401244550943, 0.07866350561380386, 0.021427830681204796, 0.06808756291866302, -0.14849570393562317, 0.12625953555107117, 0.11491861194372177, 0.0745885819196701, 0.14969949424266815, -0.13788971304893494, -0.10557665675878525, -0.020801078528165817, -0.11168765276670456, -0.012492796406149864, -0.058466535061597824, -0.0597294345498085, -0.00864311121404171, 0.1255345344543457, -0.07631349563598633, -0.03377525135874748, -0.08047254383563995, 0.043753061443567276, 0.1231519803404808, -0.055986855179071426, -0.11927749216556549, 0.14121930301189423, 0.14575301110744476, 0.1288340985774994, -0.007569280918687582, -0.059025105088949203, 0.0005597128765657544, -0.12898696959018707, 0.022146260365843773, -0.020353225991129875, -0.0734708309173584, 0.11452537775039673, 0.1119472086429596, -0.03823327645659447, 0.13290917873382568, 0.026224151253700256, -0.01817743480205536, 0.06436638534069061, -0.1328345537185669, -0.028561308979988098, -0.09305120259523392, -0.1200220137834549, -0.05508282035589218, -0.08316943794488907, -0.054660044610500336, -0.07284801453351974, 0.04977547749876976, 0.07422193139791489, 0.05765533819794655, -0.006945671048015356, -0.049668338149785995, 0.038883283734321594, 0.01573360711336136, -0.06830485165119171, 0.031734783202409744, 0.1011258140206337, -0.10182467848062515, -0.02244865521788597, -0.014713263139128685, 0.14162589609622955, -0.1311422884464264, 0.04645654559135437, -0.121200792491436, -0.1062593013048172, 0.061355020850896835, 0.14171664416790009, -0.0570724681019783, 0.13189630210399628, 0.11543791741132736, -0.09611936658620834, 0.08018719404935837, -0.026285087689757347, -0.09345492720603943, 0.013796115294098854, -0.11291421204805374, 0.08102293312549591, -0.1229468360543251, 0.12239633500576019, -0.10301874577999115, -0.10178229957818985, 0.06193909794092178, 0.02981650084257126, 0.10847288370132446, 0.13635587692260742, 0.09721183031797409, -0.07327985018491745, -0.005877287592738867, -0.08981678634881973, -0.14956876635551453, -0.04463118687272072, -0.02274511009454727, -0.05745651200413704, 0.11471386253833771, 0.0600467175245285, -0.10826423764228821, -0.007587917149066925, -0.10182943940162659, 0.009607023559510708, 0.08845146745443344, -0.06173238903284073, 0.15147267282009125, 0.037849701941013336, -0.04758424684405327, 0.10301509499549866, 0.05877789109945297, -0.054341237992048264, -0.09310748428106308, -0.06790320575237274, 0.11413100361824036, 0.1510971486568451, 0.10448206216096878, -0.13850891590118408, 0.12005165219306946, 0.091551274061203, -0.11211854964494705, 0.010489406995475292, 0.005112033803015947, -0.046575985848903656, -0.03446931391954422, 0.12754125893115997, 0.12531103193759918, 0.06892845779657364, -0.07470236718654633, 0.06150118261575699, 0.0787089616060257, 0.04397568479180336, 0.141177698969841, 0.03772721812129021, -0.09626356512308121, 0.020564313977956772, 0.14011190831661224, 0.07445673644542694, 0.0004804602940566838, 0.06673704087734222, -0.13318485021591187, -0.0779024064540863, -0.024675877764821053, -0.09750406444072723, 0.0957617312669754, 0.0827525183558464, 0.1296040266752243, -0.03180602937936783, -0.1300278753042221, -0.05608559027314186, 0.02757050283253193, -0.10980167984962463, 0.0964893102645874, 0.04352542757987976, 0.08141246438026428, -0.14982756972312927, -0.07185813784599304, 0.048392582684755325, 0.09947993606328964, 0.03533341735601425, 0.10174594074487686, -0.13837353885173798, -0.14225570857524872, 0.13157939910888672, -0.026829974725842476, -0.055743683129549026, 0.13495582342147827, -0.001561590121127665, 0.13232800364494324, 0.11529092490673065, 0.13876621425151825, 0.14100337028503418, -0.016668928787112236, 0.02057579718530178, -0.14930875599384308, -0.1408931016921997, -0.08396168798208237, -0.06452707946300507, -0.09903251379728317, 0.11033297330141068, -0.06677505373954773, -0.10975560545921326, 0.08316409587860107, -0.07829572260379791, 0.14623641967773438, 0.008638489060103893, 0.04141076281666756, -0.10212859511375427, 0.1299947053194046, -0.09144565463066101, -0.03260426968336105, -0.11730820685625076, 0.10343635827302933, -0.1334507018327713, 0.09062045812606812, 0.08075648546218872, -0.11407838761806488, -0.03378735110163689, 0.1030062735080719, 0.11906380206346512, -0.09321058541536331, -0.06510024517774582, 0.0022645113058388233, -0.07645253092050552, -0.10340674966573715, -0.07159396260976791, -0.044592514634132385, 0.12407507002353668, -0.06146199628710747, -0.11116478592157364, -0.09843737632036209, 0.04952918365597725, 0.058641985058784485, -0.10799474269151688, -0.015397383831441402, 0.08492858707904816, -0.07336339354515076, -0.0686410665512085, -0.07773499190807343, 0.024989457800984383, -0.14675205945968628, -0.11438488960266113, -0.04115881770849228, 0.13569022715091705, -0.12525363266468048, -0.00135684700217098, -0.04820440337061882, -0.010027213953435421, 0.14544352889060974, -0.00681564724072814, 0.13600623607635498, -0.05906683951616287, -0.13411198556423187, 0.14808954298496246, 0.04527422785758972, -0.1373004913330078, 0.13470147550106049, 0.0928272157907486, -0.06533118337392807, -0.14299657940864563, -0.09689100086688995, -0.14466331899166107, -0.06298499554395676, 0.035733677446842194, 0.06619970500469208, 0.09188304841518402, -0.14734432101249695, -0.11175530403852463, -0.08415098488330841, 0.13408996164798737, -0.14116063714027405, -0.0864335298538208, -0.12833252549171448, 0.13808175921440125, -0.13758154213428497, 0.057497501373291016, 0.10153163969516754, -0.087367482483387, 0.14355319738388062, -0.06905900686979294, 0.058414068073034286, 0.06317389756441116, 0.12527859210968018, 0.02613767609000206, 0.06119968369603157, 0.1049426943063736, 0.1288803219795227, -0.13786549866199493, -0.06132058426737785, -0.14259302616119385, 0.02977878786623478, 0.065593421459198, 0.12582221627235413, 0.13609479367733002, 0.035825762897729874, -0.12058623135089874, 0.049937229603528976, 0.11734911799430847, -0.02111208438873291, 0.11911014467477798, 0.12807081639766693, 0.07823070883750916, -0.13774597644805908, -0.09406299144029617, -0.037109922617673874, 0.044900793582201004, 0.0774838924407959, -0.050514791160821915, 0.04486998915672302, -0.012238978408277035, -0.12722910940647125, 0.0590520054101944, -0.08610783517360687, 0.15261586010456085, -0.08794812113046646, -0.07129254937171936, 0.03590107709169388, 0.0955711305141449, 0.03328517824411392, 0.1440850794315338, 0.060925789177417755, 0.05307070165872574, 0.00555695965886116, 0.03350869193673134, -0.13633258640766144, 0.13168519735336304, 0.019919278100132942, -0.07410302758216858, 0.10655961185693741, -0.14491762220859528, 0.14221003651618958, 0.05563876032829285, 0.046377748250961304, -0.0618274062871933, -0.00557336863130331, 0.03286729007959366, 0.14217157661914825, -0.1394309103488922, -0.13317754864692688, -0.10020095109939575, 0.017642252147197723, -0.0003849750501103699, -0.019516248255968094, 0.004106044303625822, 0.12252248823642731};

FCLayer::FCLayer() {
}

FCLayer::FCLayer(int input_sz, int output_sz, double scale, int zero_point, int batch, bool default_weight, double dropout_rate) {
    input_size = input_sz;
    output_size = output_sz;
    batch_size = batch;
    quant_zero_point = zero_point;
    quant_scale = scale;
    this->dropout_rate = dropout_rate;

    bias = new double[output_size];
    for (int j = 0; j < output_size; j++) {
        if (default_weight) {
            bias[j] = initial_model_bias[j];
        } else {
            bias[j] = (double)rand() / (RAND_MAX);
        }
    }

    weights = new double* [input_size];
    for (int i = 0; i < input_size; i++) {
        weights[i] = new double[output_size];
        for (int j = 0; j < output_size; j++) {
            weights[i][j] = initial_model_weights[j + i * output_size];
        }
    }

    output_error_softmax = new double* [batch_size];
    for (int b = 0; b < batch_size; b++) {
        output_error_softmax[b] = new double[output_size];
    }

    dropout_mask.resize(batch_size, std::vector<bool>(input_size, true));  
}

void FCLayer::apply_dropout(double** input_float) {
    if (dropout_rate > 0.0 && dropout_rate < 1.0) {
        for (int b = 0; b < batch_size; b++) {
            for (int i = 0; i < input_size; i++) {
                dropout_mask[b][i] = (rand() / (RAND_MAX + 1.0)) >= dropout_rate;
                input_float[b][i] *= dropout_mask[b][i];
            }
        }
    }
}


void FCLayer::set_weights_bias(double **new_weights, double *new_bias) {
    for(int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            weights[i][j] = new_weights[i][j];
        }        
    }

    for(int j = 0; j < output_size; j++) {
        bias[j] = new_bias[j];
    }
}

void prune_weights_hard(double** weights, double prune_percentage, int input_size, int output_size) {
    std::vector<std::pair<double, int>> flat_weights;
    flat_weights.reserve(input_size * output_size);
    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            flat_weights.push_back(std::make_pair(weights[i][j], i * output_size + j));
        }
    }

    std::sort(flat_weights.begin(), flat_weights.end(), [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
        return std::abs(a.first) < std::abs(b.first);
    });

    int total_weights = input_size * output_size;
    int num_weights_to_prune = static_cast<int>(prune_percentage * total_weights);

    for (int i = 0; i < num_weights_to_prune; ++i) {
        int index = flat_weights[i].second;
        int row = index / output_size;
        int col = index % output_size;
        weights[row][col] = 0.0;
    }
}

void softmax(double *input_output_data, int classes){
	double sum = 0;
	double max = -10000.0;
	for(int j = 0; j < classes; j++){
		if(input_output_data[j] > max){
			max = input_output_data[j];
		}
	}
	for(int j = 0; j < classes; j++){
		input_output_data[j] = exp(input_output_data[j]-max);
		sum += input_output_data[j];
	}
	for(int j = 0; j < classes; j++){
		input_output_data[j] = input_output_data[j]/sum;
	}
}

void softmax_prime(double *pred, double *result, int classes){
	for(int a = 0; a < classes; a++){
		result[a] = 0;
		for(int b = 0; b < classes; b++){
			if(a == b){
				result[a] += pred[a]*(1-pred[a]);
			}else{
				result[a] += -1*pred[a]*pred[b];
			}
		}
	}
}

void FCLayer::dequantize(int *input_data, double *input_float){
	for(int i = 0; i < input_size; i++){
		input_float[i] = (input_data[i]-quant_zero_point)*quant_scale;
	}
}


void FCLayer::forward(double** input_float, double** output) {
    apply_dropout(input_float);  

    for (int b = 0; b < batch_size; b++) {
        for (int j = 0; j < output_size; j++) {
            output[b][j] = 0.0;
            for (int i = 0; i < input_size; i++) {
                output[b][j] += input_float[b][i] * weights[i][j];
            }
            output[b][j] += bias[j];
        }
        softmax(output[b], output_size);
    }
}

double L2(double **weight, int input_size, int output_size, int batch_size){
	double sum = 0.0;
	for(int i = 0; i < input_size; i++){
		for(int j = 0; j < output_size; j++){
			sum += pow(weight[i][j],2);
		}
	}
	return sum / batch_size;
}

void cross_entropy_prime(double *pred, int *real, double *result, int classes){
	for(int j = 0; j < classes; j++){
		result[j] = -1.0*real[j]/pred[j];
	}
}

double cross_entropy_loss(double **pred, int **real, int batch_size, int classes){
	double result = 0.0;
	double epsilon = 0.0000001;

	for(int b = 0; b < batch_size; b++){
		for(int j = 0; j < classes; j++){
			if(real[b][j] == 1 and pred[b][j] < epsilon){
				result -= real[b][j]*log(epsilon);
			}else{
				result -= real[b][j]*log(pred[b][j]);
			}
		}
	}
	return result/(batch_size);
}

void mse_prime(double *pred, int *real, double *result, int classes){
	for(int i = 0; i < classes; i++){
		result[i] += 2*(pred[i]-real[i]);
	}

}

double mse(double **pred, int **real, int batch_size, int classes){
	double result = 0.0;

	for(int b = 0; b < batch_size; b++){
		for(int j = 0; j < classes; j++){
			result += pow((pred[b][j]-real[b][j]), 2);
		}
	}
	return result/(classes*batch_size);
}

void combined_ce_softmax_prime(double *pred, int *real, double *result, int classes){
	for(int i = 0; i < classes; i++){
		result[i] = pred[i] - real[i];
		assert(result[i] >= -1 && result[i] <= 1);
	}
}

double accuracy(double **pred, int **real, int batch_size, int classes){
	double correct = 0.0;

	for(int b = 0; b < batch_size; b++){
		for(int j = 0; j < classes; j++){
			if((pred[b][j] >= 0.5 && real[b][j] == 1)||(pred[b][j] < 0.5 && real[b][j] == 0)){
				correct += 1;
			}
		}
	}
	return correct/(classes*batch_size);
}

void FCLayer::backward (double **output, int **ground_truth, 
						double **input_error, double **input_float,
						double learning_rate, double lambda) {

	for(int b = 0; b < batch_size; b++) {
	    combined_ce_softmax_prime(output[b], ground_truth[b], output_error_softmax[b], output_size);
	}

	for(int b = 0; b < batch_size; b++){
	    for(int i = 0; i < input_size; i++) {
	    	input_error[b][i] = 0.0;
		    for(int j = 0; j < output_size; j++){
				input_error[b][i] += output_error_softmax[b][j]*weights[i][j]/batch_size;
		    }
		}
	}

	double weights_error[input_size][output_size];

	for(int i = 0; i < input_size; i++) {
		for(int j = 0; j < output_size; j++){
			weights_error[i][j] = 0.0;
		}
	}

	for(int i = 0; i < input_size; i++) {
		for(int j = 0; j < output_size; j++){
			for(int b = 0; b < batch_size; b++){
				weights_error[i][j] += input_float[b][i]*output_error_softmax[b][j]/batch_size;
		    }
		}
	}

	for(int i = 0; i < input_size; ++i) {
		for(int j = 0; j < output_size; j++){
			this->weights[i][j] = this->weights[i][j]*(1-learning_rate*lambda/batch_size)-learning_rate*weights_error[i][j];
		}
	}
	
	for(int j = 0; j < output_size; j++){
		for(int b = 0; b < batch_size; b++) {
			this->bias[j] -= learning_rate*output_error_softmax[b][j]/batch_size;
		}
	}
}

void FCLayer::cleanup(){
	cout << "cleaning up object\n";
	for(int i = 0; i < input_size; i++) {
		delete [] weights[i];
	}
	for(int b = 0; b < batch_size; b++){
	    delete [] output_error_softmax[b];
	}
	delete [] output_error_softmax;
	delete[] weights;
	delete[] bias;
}

FCLayer::~FCLayer() { }

void predict(double **input_float, FCLayer *model, double **output, int batch_size, int classes){
	model->forward(input_float, output);

	for(int b = 0; b < batch_size; b++){
		for(int j = 0; j < classes; j++){
			if(output[b][j] >= 0.5){
				output[b][j] = 1.0;
			}else{
				output[b][j] = 0.0;
			}
		}
	}
}

void FL_round_simulation(double **input_f, int **input_i, int **ground_truth, int local_episodes, 
                         double learning_rate, FCLayer *model, double lambda, 
                         bool verbose, bool local, bool unquantize) {
    if (verbose == true) {
        if (local == true) {
            cout << "\tstarted sim\n";
        } else {
        }
    }

    double **output = new double*[model->batch_size];
    double **input_error = new double*[model->batch_size];
    double **input_float = new double*[model->batch_size];

    if (verbose == true) {
        if (local == true) {
            cout << "\tallocated\n";
        } else {
        }
    }

    for (int b = 0; b < model->batch_size; b++) {
        output[b] = new double[model->output_size];
        for (int j = 0; j < model->output_size; j++) {
            output[b][j] = 0.0;
        }

        input_error[b] = new double[model->input_size];
        for (int i = 0; i < model->input_size; i++) {
            input_error[b][i] = 0.0;
        }
        if (unquantize == true) {
            model->dequantize(input_i[b], input_float[b]);
        } else {
            input_float[b] = input_f[b];
        }
    }

    if (verbose == true) {
        if (local == true) {
            cout << "\tallocated part 2\n";
            cout << "\tinitial bias loaded " << model->bias[0] << " " << model->bias[1] << "\n";
            cout << "\tinitial output " << output[0][0] << " " << output[0][1] << "\n";
        } else {
        }
    }

    for (int epi = 0; epi < local_episodes; epi++) {
        if (verbose == true) {
            if (local == true) {
                cout << "EPISODE " << epi << "\n";
                cout << "\tforward\n";
                cout << "\t\tbias loaded " << model->bias[0] << " " << model->bias[1] << "\n";
            } else {
            }
        }

        model->forward(input_float, output);

        if (verbose == true) {
            if (local == true) {
                cout << "\tsoftmax " << output[0][0] << " " << output[0][1] << "\n";
            } else {
            }
        }

        double l2_error = lambda * L2(model->weights, model->input_size, model->output_size, model->batch_size) / 2;
        double error = cross_entropy_loss(output, ground_truth, model->batch_size, model->output_size) + l2_error;
        double acc = accuracy(output, ground_truth, model->batch_size, model->output_size);
        if (epi == local_episodes - 1) {
            cout << " train acc: " << acc;
        }

        model->backward(output, ground_truth, input_error, input_float, learning_rate, lambda);

        if (verbose == true) {
            if (local == true) {
                cout << "\tbackward\n";
            } else {
            }
        }
    }

    if (verbose == true) {
        if (local == true) {
            cout << "\tdone loop\n";
        } else {
        }
    }

    for (int b = 0; b < model->batch_size; b++) {
        delete[] input_error[b];
        delete[] output[b];
        if (unquantize == true) {
            delete[] input_float[b];
        }
    }
    delete[] input_error;
    delete[] output;
    delete[] input_float;

    if (verbose == true) {
        if (local == true) {
            cout << "\tdone de-allocation\n";
        } else {
        }
    }
}
