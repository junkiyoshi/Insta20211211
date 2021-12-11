#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	auto radius = 12;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	for (auto y = 0; y < ofGetWidth(); y += radius * 1.5) {

		for (int x = 0; x < ofGetHeight() * 3; x += x_span) {

			glm::vec3 location;

			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			this->setHexagonToMesh(this->base_face, this->base_frame, location - glm::vec3(ofGetHeight() * 1.5, ofGetWidth() * 0.5, 0), 12, 6, glm::vec3(), ofColor(0), ofColor(255));
		}

		flg = !flg;
	}

	this->base_frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face = this->base_face;
	this->frame = this->base_frame;

	auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	for (int i = 0; i < this->face.getNumVertices(); i++) {

		auto noise_x = ofNoise( this->face.getVertex(i).x * 0.001 + ofGetFrameNum() * 0.008);
		auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_x, 0, 1, PI * -3, PI * 3), glm::vec3(1, 0, 0));

		auto tmp = glm::vec4(this->face.getVertex(i), 0) * rotation_x;

		this->face.setVertex(i, tmp);
		this->frame.setVertex(i, tmp);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(280);

	this->face.draw();
	this->frame.draw();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height, glm::vec3 deg, ofColor face_color, ofColor frame_color) {

	auto rotation_x = glm::rotate(glm::mat4(), deg.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
	auto rotation_y = glm::rotate(glm::mat4(), deg.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
	auto rotation_z = glm::rotate(glm::mat4(), deg.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

	for (int deg = 90; deg < 450; deg += 60) {

		auto face_radius = radius - 0.5;
		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(0, 0, 0));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(0, 0, height));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		for (auto& vertex : vertices) {

			vertex = location + glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		auto frame_index = frame_target.getNumVertices();

		vertices.clear();
		vertices.push_back(glm::vec3(0, 0, 0));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(0, 0, height));
		vertices.push_back(glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height));

		for (auto& vertex : vertices) {

			vertex = location + glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
		}

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}